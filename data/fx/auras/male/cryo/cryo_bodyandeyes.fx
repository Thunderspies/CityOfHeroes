#############################################################
## Cryo_Body.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Eye_GlowL.part
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:Eye_GlowR.part
		Part	:Eye_Glow.part
	End

#########################################################

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Head
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	Neck
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	Chest
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	Hips
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LarmL
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LArmR
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandR
	End

	Event
		EName	ULegLFX
		Type	PositOnly
		At	ULegL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LLegL
	End

	Event
		EName	ULegRFX
		Type	PositOnly
		At	ULegR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	LLegR
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	LLegL
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	FootL
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	LLegR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	FootR
	End
End

#########################################################

End