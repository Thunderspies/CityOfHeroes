#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
Lifespan 8

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event

		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Jaunt_Flash_Head.part
		Part	:Jaunt_Glow_Head.part
		Lifespan	1
	End
End

Condition
	On	Time
	Time	5

	Event

		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Rings_Head.part
		Part	:Spiral_Head.part
		Part	:Spiral_Head_Additive.part
		Lifespan	3
	End
End

#############################################################

End