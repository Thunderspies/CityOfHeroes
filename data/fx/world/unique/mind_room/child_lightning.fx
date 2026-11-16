#############################################################
## Sky_Flashes.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	Chance .25
	Event
		Type	Local
		At	Origin
		SoundNoRepeat 2
		Sound 	MoM_MindZap_01 500 100 .75
		Sound 	MoM_MindZap_02 500 100 .75
		Sound 	MoM_MindZap_03 500 100 .75
		Sound 	MoM_MindZap_04 500 100 .75
		Sound 	MoM_MindZap_05 500 100 .75
		Sound 	MoM_MindZap_06 500 100 .75
	End
End


Condition
	On		Time
	Time		0

	Event
		Type	Local
		At	Origin
		BhvrOverride
			StartJitter	60 20 60
		End
		Part	:Lightning_Flash.part
		Part	:Lightning_Bolt.part
		Part	:Lightning_Bolt_Glow.part
		Lifespan 1

	End
End

#############################################################

End