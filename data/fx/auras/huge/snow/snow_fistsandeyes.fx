#############################################################
## Snow_FistsAndEyes.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#########################################################

Condition
	On	Time
	Time	0

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
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_GlowL.part
		Part	:Eye_Glow.part
		Part	:SnowFlakes_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_GlowR.part
		Part	:Eye_Glow.part
		Part	:SnowFlakes_Eyes.part
	End

	Event
		Type	Local
		At	LArmL
		Part	:ColdMist_Additive_Local.part
	End

	Event
		Type	Local
		At	HandL
		Part	:ColdMist_Local.part
		Part	:ColdMist_Local_Trail.part
		Part	:Snowflakes.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local_Trail.part
	End

	Event
		Type	Local
		At	LArmR
		Part	:ColdMist_Additive_Local.part
	End

	Event
		Type	Local
		At	HandR
		Part	:ColdMist_Local.part
		Part	:ColdMist_Local_Trail.part
		Part	:Snowflakes.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local.part
		Part	:ColdMist_Additive_Local_Trail.part
	End
End

#########################################################

End