build:
	@echo "Copying ./res/ directory into $(CND_ARTIFACT_DIR_Debug)"
	cp -r ./res $(CND_ARTIFACT_DIR_Debug)
.PHONY: build
	
clean:
	rm -rf $(CND_ARTIFACT_DIR_Debug)/res