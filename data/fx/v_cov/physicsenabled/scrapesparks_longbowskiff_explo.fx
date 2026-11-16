#############################################################
## ScrapeSparks.fx
#############################################################

FxInfo

Lifespan 5

#############################################################

Condition
	Time 	0

	Event
		EName	VaultHookup
		Type	local
		At	origin
		Part	:ScrapeSparks.part
		SoundNoRepeat 5
		Sound	MetalDebris_Large_01 50 50 .8
		Sound	MetalDebris_Large_02 50 50 .8
		Sound	MetalDebris_Large_03 50 50 .8
		Sound	MetalDebris_Large_04 50 50 .8
		Sound	MetalDebris_Large_05 50 50 .8
		Sound	MetalDebris_Large_06 50 50 .8
	End
End

#############################################################

End