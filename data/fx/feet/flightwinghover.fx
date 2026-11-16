#########################################################
## 	Flight
#########################################################

FxInfo

LifeSpan 40

Condition
	Event
		Type Local
		At Root
		SoundNoRepeat 3
		Sound WingFlap_01 40 40 .2
		Sound WingFlap_02 40 40 .2
		Sound WingFlap_03 40 40 .2
		Sound WingFlap_04 40 40 .2
		Sound WingFlap_05 40 40 .2
		Sound WingFlap_06 40 40 .2
		Sound WingFlap_07 40 40 .2
	End
End

Condition  ### Sound will NOT play on wings without a particle in the script
	On Chance
	Chance .0
	Event
		Type	local 
		At	root
		Part flash.part
		lifespan 1
		# Pother	Chest
	End
End