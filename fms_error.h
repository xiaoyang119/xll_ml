// fms_error.h - error reporting
#pragma once
#include <exception>
#include <format>
#include <source_location>
#include <string>
#include <string_view>

// Convert expression to string
#define ENSURE_HASH_(x) #x
#define ENSURE_STRZ_(x) ENSURE_HASH_(x)

#ifdef _WIN32
	//#include <Windows.h>
	#define ENSURE_DEBUG_BREAK() DebugBreak()
#else
	#define ENSURE_DEBUG_BREAK() __builtin_debugtrap();
#endif // _WIN32

#ifndef ensure

// Define NENSURE to turn off ensure.
#ifdef NENSURE
	#define ensure(x) x
#else
	#if defined(_DEBUG) && defined(DEBUG_BREAK)
		#define ensure(e) if (!(e)) { \
			ENSURE_DEBUG_BREAK(); } else (void)0
	#else
		#define ensure(x) \
			if (!(x)) throw fms::error(ENSURE_STRZ_(x));
	#endif // _DEBUG && DEBUG_BREAK
#endif // NENSURE
#endif // ensure

namespace fms {

	// Error string base on souce location.
	class error : public std::exception {
		std::string message;
	public:
		// file: <file>
		// line: <line>
		//[func: <func>]
		// mesg: <mesg>
		error(const std::string_view& mesg, const std::source_location& loc = std::source_location::current())
			: message{ std::format("file: {}\nline: {}", loc.file_name(), loc.line()) }
		{
			if (loc.function_name()) {
				message.append("\nfunc: ").append(loc.function_name());
			}
			message.append("\nmesg: ").append(mesg);
		}
		error(const error&) = default;
		error& operator=(const error&) = default;
		~error() = default;

		// near: <near>
		// here: ---^
		error& at(std::string_view _near, int here = 0)
		{
			if (!_near.empty()) {
				message.append("\nnear: ").append(_near);
				if (here > 0) {
					message.append("\nhere: ").append(here, '-').append("^");
				}
			}

			return *this;
		}

		// throw error("mesg")[.at("near"[, here])];
		const char* what() const noexcept override
		{
			return message.c_str();
		}

	};


} // namespace fms