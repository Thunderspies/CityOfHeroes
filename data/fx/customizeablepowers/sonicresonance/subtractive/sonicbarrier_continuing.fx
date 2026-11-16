#############################################################
## Sonic Barrier - Continuing FX
#############################################################

FxInfo

#########################################################

Input
	Inpname	Hips
End

#########################################################

## OFFSETS #######################################################

Condition
	On 	Time
	Time	1

	Event
		EName	Hookup1
		Type	PositOnly
		At	Hips
		Bhvr	CustomizeablePowers\SonicResonance\SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 0 0
			PositionOffset	0 3 0
		End
	End

	Event
		EName	Hookup2
		Type	PositOnly
		At	Hips
		Bhvr	:SonicRing_Fade1.bhvr
		BhvrOverride
			PyrRotate	0 90 0
			PositionOffset	0 3 0
		End
	End
End

### BODY AURA 2 - Elliptical Rings ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		part1	:Aura_SonicRipples_Large1.part
	End

	Event
		Type	Local
		At	UArmL
		part1	:Aura_SonicRipples_Small1.part
		POther	LArmL
	End

	Event
		Type	Local
		At	UArmR
		part1	:Aura_SonicRipples_Small1.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LArmL
		part1	:Aura_SonicRipples_Small1.part
		POther	HandL
	End

	Event
		Type	Local
		At	LArmR
		part1	:Aura_SonicRipples_Small1.part
		POther	HandR
	End

	Event
		Type	Local
		At	ULegL
		part1	:Aura_SonicRipples_Medium1.part
		POther	LLegL
	End

	Event
		Type	Local
		At	ULegR
		part1	:Aura_SonicRipples_Medium1.part
		POther	LLegR
	End

	Event
		Type	Local
		At	LLegL
		part1	:Aura_SonicRipples_Medium1.part
		POther	FootL
	End

	Event
		Type	Local
		At	LLegR
		part1	:Aura_SonicRipples_Medium1.part
		POther	FootR
	End
End

#############################################################

End