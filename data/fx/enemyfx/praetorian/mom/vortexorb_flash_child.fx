#############################################################
## VoidOrb_Flash_Child.fx
#############################################################

FxInfo
	Flags DontSuppress

###########################################################

Condition
	On	Cycle
	Time	13
	Chance	0.5

	Event
		Type	Local
		At	Root
		SoundNoRepeat 5
		Sound 	MoM_OrbZap_01 100 100 .5
		Sound 	MoM_OrbZap_02 100 100 .5
		Sound 	MoM_OrbZap_03 100 100 .5
		Sound 	MoM_OrbZap_04 100 100 .5
		Sound 	MoM_OrbZap_05 100 100 .5
		Sound 	MoM_OrbZap_06 100 100 .5
		Sound 	MoM_OrbZap_07 100 100 .5
		Sound 	MoM_OrbZap_08 100 100 .5
		Sound 	MoM_OrbZap_09 100 100 .5
		lifespan 1
	End

	Event
		Ename	LitFlash
		At 	Origin
		Type 	Local
		BhvrOverride
			StartJitter 3 3 3
		End
		Part :VortexOrb_CloudFlash.part
		Part :VortexOrb_CloudFlash_Highlight.part
		Lifespan 3
	End

	Event
		Ename	LitFlash
		At 	Origin
		Type 	Local
		BhvrOverride
			StartJitter 0 0 0
		End
		Part :VortexOrb_Lightning.part
		Part :VortexOrb_BigFlash.part
		Lifespan 3
	End
End

Condition
	On	Cycle
	Time	10
	Chance	0.2

	Event
		Ename	LitFlash
		At 	Origin
		Type 	Local
		BhvrOverride
			StartJitter 3 3 3
		End
		Part :VortexOrb_CloudFlash.part
		Part :VortexOrb_CloudFlash_Highlight.part
		Lifespan 3
	End

	Event
		Ename	LitFlash
		At 	Origin
		Type 	Local
		BhvrOverride
			StartJitter 0 0 0
		End
		Part :VortexOrb_Lightning.part
		Part :VortexOrb_BigFlash.part
		Lifespan 3
	End
End

End