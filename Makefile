all:
	$(MAKE) -C Client all
	$(MAKE) -C Server all
	$(MAKE) -C Display all
clean:
	$(MAKE) -C Client clean
	$(MAKE) -C Server clean
	$(MAKE) -C Display clean
zip: clean
	zip -r OBS_Prog2_if14b062_if14b049.zip . -x .git/ -x .gitignore -x .git/**\* -x *.pdf -x *.in
