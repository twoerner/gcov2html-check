## Copyright (C) 2006-2011  Trevor Woerner

########################
## sgml doc/Makefile.am
########################
SUBDIRS = 
IMAGES = html.png coverage.png summary.png
EXTRA_DIST = gcov2html-check.sgml stylesheet.dsl $(IMAGES)
if BUILDDOCS
all-local:: index.html pdf txt
endif
docbasedir = @DOCBASEDIR@
sgml_htmldir = $(docbasedir)/html
sgml_pdfdir = $(docbasedir)/pdf
sgml_txtdir = $(docbasedir)/txt
stylesheet = $(srcdir)/stylesheet.dsl
INST_IMAGES = $(addprefix $(top_srcdir)/doc/,$(IMAGES))

index.html: gcov2html-check.sgml stylesheet.dsl $(INST_IMAGES)
	$(RM) *.html *.htm
	for image in $(INST_IMAGES); do \
		$(INSTALL) "$$image" .; \
	done
	$(doc_html) -d '$(stylesheet)#html' $<

gcov2html-check.pdf: gcov2html-check.sgml stylesheet.dsl $(INST_IMAGES)
	for image in $(INST_IMAGES); do \
		$(INSTALL) "$$image" .; \
	done
	$(doc_pdf) -d '$(stylesheet)#print' $<

gcov2html-check.txt: gcov2html-check.sgml stylesheet.dsl
	$(doc_txt) -d '$(stylesheet)#html' $<

pdf: gcov2html-check.pdf
txt: gcov2html-check.txt

if BUILDDOCS
install-data-local:
	$(mkdir_p) $(sgml_htmldir)
	$(mkdir_p) $(sgml_pdfdir)
	$(mkdir_p) $(sgml_txtdir)
	$(INSTALL) *.html $(sgml_htmldir)
	$(INSTALL) *.pdf $(sgml_pdfdir)
	$(INSTALL) *.txt $(sgml_txtdir)
endif

uninstall-local::
	$(RM) -fr $(docbasedir)

clean-local::
	$(RM) *.html *.htm *.pdf *.txt
	-$(RM) $(IMAGES)
