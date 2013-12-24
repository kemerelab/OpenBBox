TEMPLATE = subdirs

SUBDIRS = src

android {
} else {
    SUBDIRS += tests
    INSTALLS += htmldocs
}

CONFIG += ordered

# Documentation generation
docs.commands = cd doc/ && doxygen
QMAKE_EXTRA_TARGETS += dist docs
# Source distribution
OPENBBOXMANAGER_ARCHIVE = OpenBBoxManager-$$QDJANGO_VERSION
dist.commands = \
    $(DEL_FILE) -r $$OPENBBOXMANAGER_ARCHIVE && \
    $(MKDIR) $$OPENBBOXMANAGER_ARCHIVE && \
    git archive master | tar -x -C $$OPENBBOXMANAGER_ARCHIVE && \
    $(COPY_DIR) doc/html $$OPENBBOXMANAGER_ARCHIVE/doc && \
    tar czf $${OPENBBOXMANAGER_ARCHIVE}.tar.gz $$OPENBBOXMANAGER_ARCHIVE && \
    $(DEL_FILE) -r $$OPENBBOXMANAGER_ARCHIVE
dist.depends = docs

# Install rules
htmldocs.files = doc/html
htmldocs.path = $$PREFIX/share/doc/OpenBBox
htmldocs.CONFIG += no_check_exist directory
