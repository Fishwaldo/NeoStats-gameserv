AUTOMAKE_OPTIONS        = foreign
ACLOCAL_AMFLAGS         = -I autotools
AM_MAKEFLAGS            = -s

pkglib_LTLIBRARIES	= gamesserv.la
gamesserv_la_SOURCES	= bomb.c gamesserv.c hilo_help.c ttt.c bomb_help.c \
			  gamesserv_help.c russian.c ttt_help.c common.c \
			  hilo.c russian_help.c
noinst_HEADERS		= gamesserv.h

gamesserv_la_LDFLAGS	= -module -avoid-version

EXTRA_DIST		= autotools/ccdv.c autotools/shtool RELNOTES

dist_doc_DATA		= README.GamesServ


include $(top_srcdir)/autotools/rules.mk
