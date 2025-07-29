#ifndef HTTELEMENTS_HPP
# define HTTELEMENTS_HPP

#include <string>
#include <map>
#include <sstream>

/* Status Codes */
#define HTTP_STATUS(XX) \
  XX(100, CONTINUE,                        Continue)                        \
  \
  XX(200, OK,                              OK)                              \
  XX(201, CREATED,                         Created)                         \
  XX(202, ACCEPTED,                        Accepted)                        \
  XX(204, NO_CONTENT,                      No Content)                      \
  XX(205, RESET_CONTENT,                   Reset Content)                   \
  XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
  \
  XX(207, MULTI_STATUS_,                    Multi-Status)                   \
  XX(208, ALREADY_REPORTED_,                Already Reported)               \
  XX(226, IM_USED_,                         IM Used)                        \
  \
  XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
  XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
  XX(302, FOUND,                           Found)                           \
  XX(303, SEE_OTHER,                       See Other)                       \
  XX(304, NOT_MODIFIED,                    Not Modified)                    \
  XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
  XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
  \
  XX(400, BAD_REQUEST,                     Bad Request)                     \
  XX(403, FORBIDDEN,                       Forbidden)                       \
  XX(404, NOT_FOUND,                       Not Found)                       \
  XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
  XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
  XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
  XX(409, CONFLICT,                        Conflict)                        \
  XX(410, GONE,                            Gone)                            \
  XX(411, LENGTH_REQUIRED,                 Length Required)                 \
  XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
  XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
  XX(414, URI_TOO_LONG,                    URI Too Long)                    \
  XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
  XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
  XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
  XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
  XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
  \
  XX(422, UNPROCESSABLE_ENTITY_,            Unprocessable Entity)           \
  XX(401, UNAUTHORIZED_,                    Unauthorized)                   \
  \
  XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
  XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
  XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
  XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
  \
  XX(506, VARIANT_ALSO_NEGOTIATES_,         Variant Also Negotiates)        \
  XX(510, NOT_EXTENDED_,                    Not Extended)                   \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED_, Network Authentication Required)\


#define HTTP_METHOD(XX) \
	XX(MethodGET , GET) \
	XX(MethodHEAD , HEAD) \
	XX(MethodPOST , POST) \
	XX(MethodPUT , PUT) \
	XX(MethodDELETE , DELETE) \
	XX(MethodCONNECT , CONNECT) \
	XX(MethodOPTION , OPTION) \
	XX(MethodTRACE , TRACE)

#define HTTP_PROTOCOLS(XX) \
	XX(Http0_9 , HTTP\0.0) \
	XX(Http1_0 , HTTP\1.0) \
	XX(Http1_1 , HTTP\1.1) \
	XX(Http1_2 , HTTP\1.2) \
	XX(Http2 , HTTP\2) \
	XX(Http3 , HTTP\3)

#define HTTP_ENCODING(XX) \
	XX(chunked, chunked) \
	XX(compress, compress) \
	XX(deflate, deflate) \
	XX(gzip, gzip) \
	XX(trailers, trailers) \
	XX(Xcompress, x-compress) \
	XX(Xgzip, x-gzip) \
	XX(AllEncoding, *)

#define HTTP_HEADERS(XX)\
	XX(AcceptRanges, Accept-Ranges) \
	XX(AcceptCharset, Accept-Charset) \
	XX(AcceptEncoding, Accept-Encoding) \
	XX(AcceptLanguage, Accept-Language) \
	XX(Accept, Accept) \
	XX(Age, Age) \
	XX(Authorization, Authorization) \
	XX(CacheControl, Cache-Control) \
	XX(Connection, Connection) \
	XX(Cookie, Cookie) \
	XX(Date, Date) \
	XX(Forwarded, Forwarded) \
	XX(From, From) \
	XX(Host, Host) \
	XX(IfMatch, If-Match) \
	XX(IfModifiedSince, If-Modified-Since) \
	XX(IfNoneMatch, If-None-Match) \
	XX(IfRange, If-Range) \
	XX(IfUnmodifiedSince, If-Unmodified-Since) \
	XX(MaxForwards, Max-Forwards) \
	XX(Pragma, Pragma) \
	XX(Range, Range) \
	XX(Referer, Referer) \
	XX(Upgrade, Upgrade) \
	XX(UserAgent, User-Agent) \
	XX(Via, Via) \
	XX(Warning, Warning) \
	XX(XRequestedWith, X-Requested-With) \
	XX(DNT, DNT) \
	XX(Origin, Origin) \
	XX(AccessControlRequestMethod, Access-Control-Request-Method) \
	XX(AccessControlRequestHeaders, Access-Control-Request-Headers) \
	XX(SecFetchDest, Sec-Fetch-Dest) \
	XX(SecFetchMode, Sec-Fetch-Mode) \
	XX(SecFetchSite, Sec-Fetch-Site) \
	XX(SecFetchUser, Sec-Fetch-User) \
	XX(XFrameOptions, X-Frame-Options) \
	XX(XContentTypeOptions, X-Content-Type-Options) \
	XX(XXSSProtection, X-XSS-Protection) \
	XX(ContentType, Content-Type) \
	XX(ContentLength, Content-Length) \
	XX(ContentEncoding, Content-Encoding) \
	XX(ContentRange, Content-Range) \
	XX(ContentDisposition, Content-Disposition) \
	XX(TransferEncoding, Transfer-Encoding) \
	XX(TE, TE) \
	XX(Expect, Expect)

#define HTTP_MIME_TYPE(XX)\
	XX(Mimeaac, 	.aac,	AAC audio,											audio/aac)	\
	XX(Mimeabw, 	.abw,	AbiWord document,									application/x-abiword)\
	XX(Mimecss, 	.css,	Cascading Style Sheets (CSS),						text/css)\
	XX(Mimeavi, 	.avi, 	AVI: Audio Video Interleave,						video/x-msvideo)	\
	XX(Mimeapng, 	.apng,	Animated Portable Network Graphics (APNG) image, 	image/apng)	\
	XX(Mimearc, 	.arc, 	Archive document (multiple files embedded),			application/x-freearc)	\
	XX(Mimeavif, 	.avif,	AVIF image,											image/avif)	\
	XX(Mimeazw, 	.azw, 	Amazon Kindle eBook format,							application/vnd.amazon.ebook)	\
	XX(Mimebin, 	.bin, 	Any kind of binary data,							application/octet-stream)	\
	XX(Mimebmp, 	.bmp, 	Windows OS/2 Bitmap Graphics,						image/bmp)	\
	XX(Mimebz, 		.bz,	BZip archive,										application/x-bzip)\
	XX(Mimebz2, 	.bz2,	BZip2 archive,										application/x-bzip2)\
	XX(Mimecda, 	.cda,	CD audio,											application/x-cdf)\
	XX(Mimecsh, 	.csh,	C-Shell script,										application/x-csh)\
	XX(Mimecsv, 	.csv,	Comma-separated values (CSV),						text/csv)\
	XX(Mimedoc, 	.doc,	Microsoft Word,										application/msword)\
	XX(Mimedocx, 	.docx,	Microsoft Word (OpenXML),							application/vnd.openxmlformats-officedocument.wordprocessingml.document)\
	XX(Mimeeot, 	.eot,	MS Embedded OpenType fonts,							application/vnd.ms-fontobject)\
	XX(Mimeepub, 	.epub,	Electronic publication (EPUB),						application/epub+zip)\
	XX(Mimegz, 		.gz,	GZip Compressed Archive,							application/x-gzip)\
	XX(Mimegif, 	.gif,	Graphics Interchange Format (GIF),					image/gif)\
	XX(Mimehtm, 	.htm,	HyperText Markup Language (HTML),					text/htm)\
	XX(Mimehtml, 	.html,	HyperText Markup Language (HTML),					text/html)\
	XX(Mimeico, 	.ico,	Icon format,										image/vnd.microsoft.icon)\
	XX(Mimempkg, 	.mpkg,	Apple Installer Package,							application/vnd.apple.installer+xml)\
	XX(Mimempeg, 	.mpeg,	MPEG Video,											video/mpeg)\
	XX(Mimemjs, 	.mjs,	JavaScript module,									text/javascript)\
	XX(Mimemp3, 	.mp3,	MP3 audio,											audio/mpeg)\
	XX(Mimemid, 	.mid,	Musical Instrument Digital Interface (MIDI),		audio/x-midi)\
	XX(Mimemidi, 	.midi,	Musical Instrument Digital Interface (MIDI),		audio/x-midi)\
	XX(Mimejar, 	.jar,	Java Archive (JAR),									application/java-archive)\
	XX(Mimeics, 	.ics,	iCalendar format,									text/calendar)\
	XX(Mimejpeg, 	.jpeg,	JPEG images,										image/jpeg)\
	XX(Mimejpg, 	.jpg,	JPEG images,										image/jpeg)\
	XX(Mimejs, 		.js,		JavaScript,										text/javascript)\
	XX(Mimejson, 	.json,	JSON format,										application/json)\
	XX(Mimejsonld, 	.jsonld,	JSON-LD format,									application/ld+json)\
	XX(Mimemp4, 	.mp4,	MP4 video,											video/mp4)\
	XX(Mimeodp, 	.odp,	OpenDocument presentation document,					application/vnd.oasis.opendocument.presentation)\
	XX(Mimeods, 	.ods,	OpenDocument spreadsheet document,					application/vnd.oasis.opendocument.spreadsheet)\
	XX(Mimeodt, 	.odt,	OpenDocument text document,							application/vnd.oasis.opendocument.text)\
	XX(Mimeoga, 	.oga,	Ogg audio,											audio/ogg)\
	XX(Mimeogv, 	.ogv,	Ogg video,											video/ogg)\
	XX(Mimeogx, 	.ogx,	Ogg,												application/ogg)\
	XX(Mimeopus, 	.opus,	Opus audio in Ogg container,						audio/ogg)\
	XX(Mimeotf, 	.otf,	OpenType font,										font/otf)\
	XX(Mimepng, 	.png,	Portable Network Graphics,							image/png)\
	XX(Mimepdf, 	.pdf,	Adobe Portable Document Format (PDF),				application/pdf)\
	XX(Mimephp, 	.php,	Hypertext Preprocessor (Personal Home Page),		application/x-httpd-php)\
	XX(Mimeppt, 	.ppt,	Microsoft PowerPoint,								application/vnd.ms-powerpoint)\
	XX(Mimepptx, 	.pptx,	Microsoft PowerPoint (OpenXML),						application/vnd.openxmlformats-officedocument.presentationml.presentation)\
	XX(Mimerar, 	.rar,	RAR archive,										application/vnd.rar)\
	XX(Mimertf, 	.rtf,	Rich Text Format (RTF),								application/rtf)\
	XX(Mimerpy, 	.py,	Python Script,										application/py)\
	XX(Mimesh, 		.sh,	Bourne shell script,								application/x-sh)\
	XX(Mimesvg, 	.svg,	Scalable Vector Graphics (SVG),						image/svg+xml)\
	XX(Mimetar, 	.tar,	Tape Archive (TAR),									application/x-tar)\
	XX(Mimetif, 	.tif,	Tagged Image File Format (TIFF),					image/tiff)\
	XX(Mimetiff, 	.tiff,	Tagged Image File Format (TIFF),					image/tiff)\
	XX(Mimets, 		.ts,	MPEG transport stream,								video/mp2t)\
	XX(Mimettf, 	.ttf,	TrueType Font,										font/ttf)\
	XX(Mimetxt, 	.txt,	Text (generally ASCII or ISO 8859-n),				text/plain)\
	XX(Mimevsd, 	.vsd,	Microsoft Visio,									application/vnd.visio)\
	XX(Mimewav, 	.wav,	Waveform Audio Format,								audio/wav)\
	XX(Mimeweba, 	.weba,	WEBM audio,											audio/webm)\
	XX(Mimewebm, 	.webm,	WEBM video,											video/webm)\
	XX(Mimewebp, 	.webp,	WEBP image,											image/webp)\
	XX(Mimewoff, 	.woff,	Web Open Font Format (WOFF),						font/woff)\
	XX(Mimewoff2, 	.woff2,	Web Open Font Format (WOFF),						font/woff2)\
	XX(Mimexhtml, 	.xhtml,	XHTML,												application/xhtml+xml)\
	XX(Mimexls, 	.xls,	Microsoft Excel,									application/vnd.ms-excel)\
	XX(Mimexlsx, 	.xlsx,	Microsoft Excel (OpenXML),							application/vnd.openxmlformats-officedocument.spreadsheetml.sheet)\
	XX(Mimexml, 	.xml,	XML,												application/xml)\
	XX(Mimexul, 	.xul,	XUL,												application/vnd.mozilla.xul+xml)\
	XX(Mimezip, 	.zip,	ZIP archive,										application/zip)\
	XX(Mimef3gp, 	.3gp,	3GPP audio/video container,							video/3gpp)\
	XX(Mimef3g2, 	.3g2,	3GPP2 audio/video container,						video/3gpp2)\
	XX(Mimef7z, 	 .7z,	7-zip archive,										application/x-7z-compressed)\
	XX(MimeUnknown,   No,   unknown,                                            *)

#define HTTP_LANGUAGE_TAG(XX) \
	XX(af,         af,        Afrikaans) \
	XX(afZA,       af-ZA,     Afrikaans (South Africa)) \
	XX(ar,         ar,        Arabic) \
	XX(arAE,       ar-AE,     Arabic (U.A.E.)) \
	XX(arBH,       ar-BH,     Arabic (Bahrain)) \
	XX(arDZ,       ar-DZ,     Arabic (Algeria)) \
	XX(arEG,       ar-EG,     Arabic (Egypt)) \
	XX(arIQ,       ar-IQ,     Arabic (Iraq)) \
	XX(arJO,       ar-JO,     Arabic (Jordan)) \
	XX(arKW,       ar-KW,     Arabic (Kuwait)) \
	XX(arLB,       ar-LB,     Arabic (Lebanon)) \
	XX(arLY,       ar-LY,     Arabic (Libya)) \
	XX(arMA,       ar-MA,     Arabic (Morocco)) \
	XX(arOM,       ar-OM,     Arabic (Oman)) \
	XX(arQA,       ar-QA,     Arabic (Qatar)) \
	XX(arSA,       ar-SA,     Arabic (Saudi Arabia)) \
	XX(arSY,       ar-SY,     Arabic (Syria)) \
	XX(arTN,       ar-TN,     Arabic (Tunisia)) \
	XX(arYE,       ar-YE,     Arabic (Yemen)) \
	XX(az,         az,        Azeri (Latin)) \
	XX(azAZ,       az-AZ,     Azeri (Latin) (Azerbaijan)) \
	XX(azCyrlAZ,   az-Cyrl-AZ,Azeri (Cyrillic) (Azerbaijan)) \
	XX(be,         be,        Belarusian) \
	XX(beBY,       be-BY,     Belarusian (Belarus)) \
	XX(bg,         bg,        Bulgarian) \
	XX(bgBG,       bg-BG,     Bulgarian (Bulgaria)) \
	XX(bsBA,       bs-BA,     Bosnian (Bosnia and Herzegovina)) \
	XX(ca,         ca,        Catalan) \
	XX(caES,       ca-ES,     Catalan (Spain)) \
	XX(cs,         cs,        Czech) \
	XX(csCZ,       cs-CZ,     Czech (Czech Republic)) \
	XX(cy,         cy,        Welsh) \
	XX(cyGB,       cy-GB,     Welsh (United Kingdom)) \
	XX(da,         da,        Danish) \
	XX(daDK,       da-DK,     Danish (Denmark)) \
	XX(de,         de,        German) \
	XX(deAT,       de-AT,     German (Austria)) \
	XX(deCH,       de-CH,     German (Switzerland)) \
	XX(deDE,       de-DE,     German (Germany)) \
	XX(deLI,       de-LI,     German (Liechtenstein)) \
	XX(deLU,       de-LU,     German (Luxembourg)) \
	XX(dv,         dv,        Divehi) \
	XX(dvMV,       dv-MV,     Divehi (Maldives)) \
	XX(el,         el,        Greek) \
	XX(elGR,       el-GR,     Greek (Greece)) \
	XX(en,         en,        English) \
	XX(enAU,       en-AU,     English (Australia)) \
	XX(enBZ,       en-BZ,     English (Belize)) \
	XX(enCA,       en-CA,     English (Canada)) \
	XX(enCB,       en-CB,     English (Caribbean)) \
	XX(enGB,       en-GB,     English (United Kingdom)) \
	XX(enIE,       en-IE,     English (Ireland)) \
	XX(enJM,       en-JM,     English (Jamaica)) \
	XX(enNZ,       en-NZ,     English (New Zealand)) \
	XX(enPH,       en-PH,     English (Republic of the Philippines)) \
	XX(enTT,       en-TT,     English (Trinidad and Tobago)) \
	XX(enUS,       en-US,     English (United States)) \
	XX(enZA,       en-ZA,     English (South Africa)) \
	XX(enZW,       en-ZW,     English (Zimbabwe)) \
	XX(eo,         eo,        Esperanto) \
	XX(es,         es,        Spanish) \
	XX(esAR,       es-AR,     Spanish (Argentina)) \
	XX(esBO,       es-BO,     Spanish (Bolivia)) \
	XX(esCL,       es-CL,     Spanish (Chile)) \
	XX(esCO,       es-CO,     Spanish (Colombia)) \
	XX(esCR,       es-CR,     Spanish (Costa Rica)) \
	XX(esDO,       es-DO,     Spanish (Dominican Republic)) \
	XX(esEC,       es-EC,     Spanish (Ecuador)) \
	XX(esES,       es-ES,     Spanish (Spain)) \
	XX(esGT,       es-GT,     Spanish (Guatemala)) \
	XX(esHN,       es-HN,     Spanish (Honduras)) \
	XX(esMX,       es-MX,     Spanish (Mexico)) \
	XX(esNI,       es-NI,     Spanish (Nicaragua)) \
	XX(esPA,       es-PA,     Spanish (Panama)) \
	XX(esPE,       es-PE,     Spanish (Peru)) \
	XX(esPR,       es-PR,     Spanish (Puerto Rico)) \
	XX(esPY,       es-PY,     Spanish (Paraguay)) \
	XX(esSV,       es-SV,     Spanish (El Salvador)) \
	XX(esUY,       es-UY,     Spanish (Uruguay)) \
	XX(esVE,       es-VE,     Spanish (Venezuela)) \
	XX(et,         et,        Estonian) \
	XX(etEE,       et-EE,     Estonian (Estonia)) \
	XX(eu,         eu,        Basque) \
	XX(euES,       eu-ES,     Basque (Spain)) \
	XX(fa,         fa,        Farsi) \
	XX(faIR,       fa-IR,     Farsi (Iran)) \
	XX(fi,         fi,        Finnish) \
	XX(fiFI,       fi-FI,     Finnish (Finland)) \
	XX(fo,         fo,        Faroese) \
	XX(foFO,       fo-FO,     Faroese (Faroe Islands)) \
	XX(fr,         fr,        French) \
	XX(frBE,       fr-BE,     French (Belgium)) \
	XX(frCA,       fr-CA,     French (Canada)) \
	XX(frCH,       fr-CH,     French (Switzerland)) \
	XX(frFR,       fr-FR,     French (France)) \
	XX(frLU,       fr-LU,     French (Luxembourg)) \
	XX(frMC,       fr-MC,     French (Principality of Monaco)) \
	XX(gl,         gl,        Galician) \
	XX(glES,       gl-ES,     Galician (Spain)) \
	XX(gu,         gu,        Gujarati) \
	XX(guIN,       gu-IN,     Gujarati (India)) \
	XX(he,         he,        Hebrew) \
	XX(heIL,       he-IL,     Hebrew (Israel)) \
	XX(hi,         hi,        Hindi) \
	XX(hiIN,       hi-IN,     Hindi (India)) \
	XX(hr,         hr,        Croatian) \
	XX(hrBA,       hr-BA,     Croatian (Bosnia and Herzegovina)) \
	XX(hrHR,       hr-HR,     Croatian (Croatia)) \
	XX(hu,         hu,        Hungarian) \
	XX(huHU,       hu-HU,     Hungarian (Hungary)) \
	XX(hy,         hy,        Armenian) \
	XX(hyAM,       hy-AM,     Armenian (Armenia)) \
	XX(id,         id,        Indonesian) \
	XX(idID,       id-ID,     Indonesian (Indonesia)) \
	XX(is,         is,        Icelandic) \
	XX(isIS,       is-IS,     Icelandic (Iceland)) \
	XX(it,         it,        Italian) \
	XX(itCH,       it-CH,     Italian (Switzerland)) \
	XX(itIT,       it-IT,     Italian (Italy)) \
	XX(ja,         ja,        Japanese) \
	XX(jaJP,       ja-JP,     Japanese (Japan)) \
	XX(ka,         ka,        Georgian) \
	XX(kaGE,       ka-GE,     Georgian (Georgia)) \
	XX(kk,         kk,        Kazakh) \
	XX(kkKZ,       kk-KZ,     Kazakh (Kazakhstan)) \
	XX(kn,         kn,        Kannada) \
	XX(knIN,       kn-IN,     Kannada (India)) \
	XX(ko,         ko,        Korean) \
	XX(koKR,       ko-KR,     Korean (Korea)) \
	XX(kok,        kok,       Konkani) \
	XX(kokIN,      kok-IN,    Konkani (India)) \
	XX(ky,         ky,        Kyrgyz) \
	XX(kyKG,       ky-KG,     Kyrgyz (Kyrgyzstan)) \
	XX(lt,         lt,        Lithuanian) \
	XX(ltLT,       lt-LT,     Lithuanian (Lithuania)) \
	XX(lv,         lv,        Latvian) \
	XX(lvLV,       lv-LV,     Latvian (Latvia)) \
	XX(mi,         mi,        Maori) \
	XX(miNZ,       mi-NZ,     Maori (New Zealand)) \
	XX(mk,         mk,        FYRO Macedonian) \
	XX(mkMK,       mk-MK,     FYRO Macedonian (Former Yugoslav Republic of Macedonia)) \
	XX(mn,         mn,        Mongolian) \
	XX(mnMN,       mn-MN,     Mongolian (Mongolia)) \
	XX(mr,         mr,        Marathi) \
	XX(mrIN,       mr-IN,     Marathi (India)) \
	XX(ms,         ms,        Malay) \
	XX(msBN,       ms-BN,     Malay (Brunei Darussalam)) \
	XX(msMY,       ms-MY,     Malay (Malaysia)) \
	XX(mt,         mt,        Maltese) \
	XX(mtMT,       mt-MT,     Maltese (Malta)) \
	XX(nb,         nb,        Norwegian (Bokm?l)) \
	XX(nbNO,       nb-NO,     Norwegian (Bokm?l) (Norway)) \
	XX(nl,         nl,        Dutch) \
	XX(nlBE,       nl-BE,     Dutch (Belgium)) \
	XX(nlNL,       nl-NL,     Dutch (Netherlands)) \
	XX(nnNO,       nn-NO,     Norwegian (Nynorsk) (Norway)) \
	XX(ns,         ns,        Northern Sotho) \
	XX(nsZA,       ns-ZA,     Northern Sotho (South Africa)) \
	XX(pa,         pa,        Punjabi) \
	XX(paIN,       pa-IN,     Punjabi (India)) \
	XX(pl,         pl,        Polish) \
	XX(plPL,       pl-PL,     Polish (Poland)) \
	XX(ps,         ps,        Pashto) \
	XX(psAR,       ps-AR,     Pashto (Afghanistan)) \
	XX(pt,         pt,        Portuguese) \
	XX(ptBR,       pt-BR,     Portuguese (Brazil)) \
	XX(ptPT,       pt-PT,     Portuguese (Portugal)) \
	XX(qu,         qu,        Quechua) \
	XX(quBO,       qu-BO,     Quechua (Bolivia)) \
	XX(quEC,       qu-EC,     Quechua (Ecuador)) \
	XX(quPE,       qu-PE,     Quechua (Peru)) \
	XX(ro,         ro,        Romanian) \
	XX(roRO,       ro-RO,     Romanian (Romania)) \
	XX(ru,         ru,        Russian) \
	XX(ruRU,       ru-RU,     Russian (Russia)) \
	XX(sa,         sa,        Sanskrit) \
	XX(saIN,       sa-IN,     Sanskrit (India)) \
	XX(se,         se,        Sami) \
	XX(seFI,       se-FI,     Sami (Finland)) \
	XX(seNO,       se-NO,     Sami (Norway)) \
	XX(seSE,       se-SE,     Sami (Sweden)) \
	XX(sk,         sk,        Slovak) \
	XX(skSK,       sk-SK,     Slovak (Slovakia)) \
	XX(sl,         sl,        Slovenian) \
	XX(slSI,       sl-SI,     Slovenian (Slovenia)) \
	XX(sq,         sq,        Albanian) \
	XX(sqAL,       sq-AL,     Albanian (Albania)) \
	XX(srBA,       sr-BA,     Serbian (Latin) (Bosnia and Herzegovina)) \
	XX(srCyrlBA,   sr-Cyrl-BA,Serbian (Cyrillic) (Bosnia and Herzegovina)) \
	XX(srSP,       sr-SP,     Serbian (Latin) (Serbia and Montenegro)) \
	XX(srCyrlSP,   sr-Cyrl-SP,Serbian (Cyrillic) (Serbia and Montenegro)) \
	XX(sv,         sv,        Swedish) \
	XX(svFI,       sv-FI,     Swedish (Finland)) \
	XX(svSE,       sv-SE,     Swedish (Sweden)) \
	XX(sw,         sw,        Swahili) \
	XX(swKE,       sw-KE,     Swahili (Kenya)) \
	XX(syr,        syr,       Syriac) \
	XX(syrSY,      syr-SY,    Syriac (Syria)) \
	XX(ta,         ta,        Tamil) \
	XX(taIN,       ta-IN,     Tamil (India)) \
	XX(te,         te,        Telugu) \
	XX(teIN,       te-IN,     Telugu (India)) \
	XX(th,         th,        Thai) \
	XX(thTH,       th-TH,     Thai (Thailand)) \
	XX(tl,         tl,        Tagalog) \
	XX(tlPH,       tl-PH,     Tagalog (Philippines)) \
	XX(tn,         tn,        Tswana) \
	XX(tnZA,       tn-ZA,     Tswana (South Africa)) \
	XX(tr,         tr,        Turkish) \
	XX(trTR,       tr-TR,     Turkish (Turkey)) \
	XX(tt,         tt,        Tatar) \
	XX(ttRU,       tt-RU,     Tatar (Russia)) \
	XX(ts,         ts,        Tsonga) \
	XX(uk,         uk,        Ukrainian) \
	XX(ukUA,       uk-UA,     Ukrainian (Ukraine)) \
	XX(ur,         ur,        Urdu) \
	XX(urPK,       ur-PK,     Urdu (Islamic Republic of Pakistan)) \
	XX(uz,         uz,        Uzbek (Latin)) \
	XX(uzUZ,       uz-UZ,     Uzbek (Latin) (Uzbekistan)) \
	XX(uzCyrlUZ,   uz-Cyrl-UZ,Uzbek (Cyrillic) (Uzbekistan)) \
	XX(vi,         vi,        Vietnamese) \
	XX(viVN,       vi-VN,     Vietnamese (Viet Nam)) \
	XX(xh,         xh,        Xhosa) \
	XX(xhZA,       xh-ZA,     Xhosa (South Africa)) \
	XX(zh,         zh,        Chinese) \
	XX(zhCN,       zh-CN,     Chinese (S)) \
	XX(zhHK,       zh-HK,     Chinese (Hong Kong)) \
	XX(zhMO,       zh-MO,     Chinese (Macau)) \
	XX(zhSG,       zh-SG,     Chinese (Singapore)) \
	XX(zhTW,       zh-TW,     Chinese (T)) \
	XX(zu,         zu,        Zulu) \
	XX(zuZA,       zu-ZA,     Zulu (South Africa))\
	XX(AllLanguage,    *,     Any Language)

#define HTTP_CGI_VARIABLE(XX) \
	XX(CgiAuthType, AUTH_TYPE) \
	XX(CgiContentLength, CONTENT_LENGTH) \
	XX(CgiContentType, CONTENT_TYPE) \
	XX(CgiGatewayInterface, GATEWAY_INTERFACE) \
	XX(CgiPathInfo, PATH_INFO) \
	XX(CgiPathTranslated, PATH_TRANSLATED) \
	XX(CgiHTTPAccept, HTTP_ACCEPT) \
	XX(CgiHTTPConnection, HTTP_CONNECTION) \
	XX(CgiHTTPAgent, HTTP_USER_AGENT) \
	XX(CgiQueryString, QUERY_STRING) \
	XX(CgiRemoteAddr, REMOTE_ADDR) \
	XX(CgiRemoteHost, REMOTE_HOST) \
	XX(CgiRemoteIdent, REMOTE_IDENT) \
	XX(CgiRemoteUser, REMOTE_USER) \
	XX(CgiRequestMethod, REQUEST_METHOD) \
	XX(CgiScriptName, SCRIPT_NAME) \
	XX(CgiServerName, SERVER_NAME) \
	XX(CgiServerPort, SERVER_PORT) \
	XX(CgiDocumentRoot, DOCUMENT_ROOT) \
	XX(CgiServerProtocol, SERVER_PROTOCOL) \
	XX(CgiServerSoftware, SERVER_SOFTWARE)


const std::string HTTP_newline = "\r\n";
const std::string HTTP_double_newline = "\r\n\r\n";

struct HTTP {

	enum StatusCode
	{
	    #define XX(StatusCodeIdentifier, StatusCodeString, StatusCodeMessage) \
	        StatusCodeString = StatusCodeIdentifier,
	        HTTP_STATUS(XX)
	    #undef XX
	    NUM_STATUS_CODE
	};

	enum Method
	{
		UnknownMethod = -1,
		#define XX(MethodIdentifier, MethodString) \
			MethodIdentifier,
			HTTP_METHOD(XX)
		#undef XX
		NUM_METHODS
	};
	static const std::map<std::string, Method> _MethodToStr;

	enum Protocol
	{
		UnknownProtocol = -1,
		#define XX(ProtocolIdentifier, ProtocolString) \
			ProtocolIdentifier,
			HTTP_PROTOCOLS(XX)
		#undef XX
		NUM_PROTOCOLS
	};
	static const std::map<std::string, Protocol> _ProtocolToStr;

	enum RequestHeader
	{
		UnknownHeader = -1,
		#define XX(HeadersIdentifier, HeadersString) \
			HeadersIdentifier,
			HTTP_HEADERS(XX)
		#undef XX
		NUM_HEADERS
	};
	static const std::map<std::string, RequestHeader> _RequestHeaderToStr;
	
	enum LanguageTag
	{
		UnknownLanguageTag = -1,
		#define XX(LanguageTagIdentifier, LanguageTagString, LanguageTagMessage) \
			LanguageTagIdentifier,
			HTTP_LANGUAGE_TAG(XX)
		#undef XX
		NUM_LENGUAGE_TAG
	};
	static const std::map<std::string, LanguageTag> _LanguageTagToStr;

	enum Encoding
	{
		UnknownEncoding = -1,
		#define XX(EncodingIdentifier, EncodingString) \
			EncodingIdentifier,
			HTTP_ENCODING(XX)
		#undef XX
		NUM_HTTP_ENCODING
	};
	static const std::map<std::string, Encoding> _EncodingToStr;

	enum Schema
	{
		SchemaHttp
	};

	enum HostType
	{
		IPv4,
		IPv6,
		HostName
	};

	enum MimeType
	{
		UnknownMimeType = -1,
		#define XX(MimeTypeIdentifier, MimeTypeExtension, MimeTypeDef, MimeTypeString) \
			MimeTypeIdentifier,
			HTTP_MIME_TYPE(XX)
		#undef XX
		NUM_MIME_TYPE
	};

	enum CgiVarible
	{
		UnknownCgiVarible = -1,
		#define XX(CgiVaribleIdentifier, CgiVaribleString) \
			CgiVaribleIdentifier,
			HTTP_CGI_VARIABLE(XX)
		#undef XX
		NUM_CGI_VARIABLE
	};

	static HTTP::CgiVarible	StrToCGIVariable(const std::string &cgi_variable);
	static std::string		CGIVariableToString(const HTTP::CgiVarible &cgi_id);

	static std::string		StatusCodeToString(const StatusCode &status);
	static HTTP::StatusCode	StrToStatusCode(const std::string &status);
	static std::string		GetCodeMessage(const StatusCode &status);

	static Method			StrToMethodId(const std::string &method);
	static std::string		MethodIdToString(const Method &method);

	static Protocol			StrToProtocolId(const std::string &protocol);
	static std::string		ProtocolIdToString(const Protocol &protocol);

	static RequestHeader	StrToRequestHeaderId(const std::string &reqHeader);
	static std::string		RequestHeaderIdToString(const RequestHeader &reqHeader);

	static LanguageTag		StrToLanguageTagId(const std::string &reqHeader);
	static std::string		LanguageTagIdToString(const LanguageTag &reqHeader);

	static Encoding			StrToEncodingId(const std::string &Encoding);
	static std::string		EncodingIdToString(const Encoding &Encoding);

	static MimeType			StrToMimeTypeId(const std::string &MimeStr);
	static std::string		MimeTypeIdToString(const MimeType &MimeId);

	static MimeType			ExtensionToMimeTypeId(const std::string &MimeStr);
	static std::string		MimeTypeIdToExtension(const HTTP::MimeType &MimeType);

	/* Structures */
	enum HTTPResult
	{
		REQUEST_ERROR,
		REQUEST_HEADERS_END,
		REQUEST_CONTINUE
	};
	
	enum RequestBody
	{
		WITH_BOUNDARY_BODY,
		CHUNKED_BODY,
		RAW_BODY,
		NUM_REQUEST_BODY
	};

	enum RequestParam
	{
		HOST_TYPE,
		HOST_PARAM,
		USER_INFO,
		AUTHORITY,
		PATH,
		QUERY,
		FRAGMENT,
		FILE_NAME,
		BODY_BOUNDARY,
		HEADERS_STRING,
		BODY_STRING,
		CONTENT,
		FULL_CHUNKED_BODY,
		FULL_PATH,
		PORT,
		DIR_PATH,
		FILE_PATH,
		NUM_PARAMS
	};

	enum RequestFlag
	{
		IS_METHOD_NOT_ALLOWED,
		CLOSE_CONNECTION,
		IS_CHUNKED,
		CHECK_MEMTYPE,
		HAS_BODY,
		IS_LAST_CHUNK,
		IS_COMPLETE_CHUNK,
		IS_CGI,
		REQUEST_FLAG_NUM
	};

	static const std::size_t CHUNK_SIZE = 30000;
};

#endif // HTTELEMENTS_HPP

