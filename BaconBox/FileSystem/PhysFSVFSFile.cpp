#include "PhysFSVFSFile.h"
#include "BaconBox/Console.h"

using namespace BaconBox;

std::string PhysFSVFSFile::write_mount_point;

void PhysFSVFSFile::setWriteMount(const std::string &where) {
	PhysFSVFSFile::write_mount_point = where;
}

PhysFSVFSFile::PhysFSVFSFile(const std::string& path, const std::string& mode) : File(path, mode), internal_file(NULL) {
	// When in w or a, the file to open must have the 'writedir' prefix removed,
	// as PhysFS does not resolve to the mounted point.
	// In PhysicsFS, a saveDir is a complete separated concept from the other directories.
	// Files in openWrite/openAppend are all absolute to that dir.

	std::string write_path;
	if (mode == "w" || mode == "wb" || mode == "a" || mode == "ab") {
		if ( path.substr(0, write_mount_point.length()) != write_mount_point ) {
#ifdef BB_DEBUG
			Console__error("Could not open file `" +path+ "` for writing or appending.");
			Console__error("Its path is not in the write mount point : " + write_mount_point);
#endif
			return;
		}
		write_path = path.substr(write_mount_point.length());
	}

	if (mode == "r" || mode == "rb") {
		internal_file = PHYSFS_openRead(path.c_str());
	}
	else if (mode == "w" || mode == "wb") {
		internal_file = PHYSFS_openWrite(write_path.c_str());
	}
	else if (mode == "a" || mode == "ab") {
		internal_file = PHYSFS_openAppend(write_path.c_str());
	}
	else {
#ifdef BB_DEBUG
		Console__error("Could not open file `" +path+ "`.");
		Console__error("Mode `"+mode+"` is invalid. for PhysFSVFSFile.");
#endif
		return;
	}
	if (!internal_file) {
		// TODO : Release-mode logger.
#ifdef BB_DEBUG
		PRLN("Opening of `"+path+"` failed.");
		PRLN("Reason: '" << PHYSFS_getLastError() << "'");
#endif
	}
}

File* PhysFSVFSFile::open(const std::string& path, const std::string& mode) {
	PhysFSVFSFile * file = new PhysFSVFSFile(path, mode);
	if (file->internal_file) {
		return file;
	}
	return NULL;
}

PhysFSVFSFile::~PhysFSVFSFile() {
	this->close();
}

void PhysFSVFSFile::close() {
	// Bail out if file is already closed.
	if (!internal_file) {
		return;
	}
	if (PHYSFS_close(internal_file) == 0) {
		#ifdef BB_DEBUG
		PRLN("Closing of file `"<< this->path <<"` failed.");
		PRLN("Reason: '" << PHYSFS_getLastError() << "'");
		#endif
	}
	internal_file = NULL;
}

size_t PhysFSVFSFile::size() {
	PHYSFS_sint64 len = 0;
	len = PHYSFS_fileLength(internal_file);
	return len;
}

int PhysFSVFSFile::seek(unsigned int offset) {
	int ret = PHYSFS_seek(internal_file, offset);
	if (ret == 0) {
		return offset;
	}
	// TODO : Error handling/logging.
	return -1;
}

int PhysFSVFSFile::tell() {
	int ret = PHYSFS_tell(internal_file);
	if (ret >= 0) {
		return ret;
	}
	// TODO : Error handling/logging.
	return -1;
}

unsigned char* PhysFSVFSFile::toBuffer(unsigned int offset, unsigned int length) {
	// If the length is zero, assume read all.
	if (length == 0) {
		PHYSFS_sint64 max_length = size();
		length = max_length - offset;
	}
	// If still, the length is zero, this is a zero-length read.
	if (length == 0) {
		return NULL;
	}
	unsigned char* buf = new unsigned char [length]();
	PHYSFS_seek(internal_file, offset);
	int amount_read = PHYSFS_read(internal_file, buf, 1, length);
	if (amount_read < 0) {
		PRLN(PHYSFS_getLastError());
		return NULL;
	}
	return buf;
}

int PhysFSVFSFile::fillBuffer(void* buffer, unsigned int offset, unsigned int length) {
	PHYSFS_seek(internal_file, offset);
	return PHYSFS_read(internal_file, buffer, 1, length);
}

std::string PhysFSVFSFile::read(unsigned int to_read) {
	unsigned int bytes_left = size() - tell();
	if (to_read == 0 || to_read > bytes_left) {
		to_read = bytes_left;
	}
	char * buf = (char*)toBuffer(tell(), to_read);
	// On empty read, the buf* is NULL.
	if (!buf) { buf = new char[1](); }
	std::string str(buf);
	delete[] buf;
	return str;
}

bool PhysFSVFSFile::write(const std::string data) {
	// Writing the whole file at once.
	const int obj_count = 1;
	int ret = PHYSFS_write(internal_file, data.c_str(), data.length(), obj_count);
	if (ret < obj_count) {
		// TODO : Release-mode logger.
#ifdef BB_DEBUG
		PRLN("Writing to `"+path+"` failed.");
		PRLN("Reason: '" << PHYSFS_getLastError() << "'");
#endif
		return false;
	}
	return true;
}

#ifdef BB_ANDROID
#include "BaconBox/Helper/Android/AndroidHelper.h"
#include <android/asset_manager.h>
#include <cassert>

int PhysFSVFSFile::toAndroidFD(off_t *outStart, off_t *outLength) {
	// TODO: Verify that this is from the APK or from an OBB.
	// PHYSFS_getRealDir(this->path.c_str());

	// First, find out what's the file underlying.
	std::string filename = this->path;

	// For now, assume the apk is mounted to /.
	// Otherwise, we'll need to figure out a way to find the mount points.
	// If it was mounted elsewhere than /, we would need to remove the prefixed part.

	PRLN("toAndroidFD : ");
	PRLN(filename);

	// use asset manager to open asset by filename
	AAssetManager* mgr = AndroidHelper::getAssetManager();
	assert(NULL != mgr);
	AAsset* asset = AAssetManager_open(mgr, filename.c_str(), AASSET_MODE_UNKNOWN);

	// FIXME : Verify error handling...
	//	// the asset might not be found
	//	if (NULL == asset) {
	//		return JNI_FALSE;
	//	}

	int fd = AAsset_openFileDescriptor(asset, outStart, outLength);
	AAsset_close(asset);

	return fd;
}

#endif
