#############################################################
## LichenHeal_Attack.fx
#############################################################

FxInfo
LifeSpan 30

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:LichenHeal_Rings.part
		Part	:LichenHeal_Spores.part
		Part	:LichenHeal_Dust.part
		Lifespan 50
	End

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 1.5 0
			Scale			0.4 0.4 0.4
			ScaleRate		1.05 1.05 1.05
 			EndScale		1.5 1.5 1.5
		End
		Part	:LichenHeal_Dust_Upright.part
		Part	:Nano_Dots_Rising.part

	End

#############################################################

End