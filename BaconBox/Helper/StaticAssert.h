/**
 * @file
 * @ingroup Helper
 */
#ifndef BB_STATIC_ASSERT_H
#define BB_STATIC_ASSERT_H

namespace BaconBox {
	template<bool Condition, typename Dummy = void>
	struct StaticAssert {
		typedef bool Result;
	};
	
	template<typename IntentionalError>
	struct StaticAssert<false, IntentionalError> {
		typedef typename IntentionalError::DoesNotExist Result;
	};
}

#endif
