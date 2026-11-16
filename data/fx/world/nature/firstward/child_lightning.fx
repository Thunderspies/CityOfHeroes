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
	On		Time
	Time		0

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			StartJitter	1500 0 1500
		End
		Part	:Lightning_Flash.part
		Part	:Lightning_Bolt.part
		Part	:Lightning_Bolt_Glow.part
		Lifespan 1

	End
End

#############################################################

End