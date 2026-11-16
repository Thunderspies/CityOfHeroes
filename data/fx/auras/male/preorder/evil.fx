#############################################################
## Evil.fx
#############################################################

FxInfo

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Chest
#		BhvrOverride
#			PositionOffset		0 0.25 0
#		End
#		Part	:Evil_LightGlow.part
#		Part	:Evil_Lightbeams.part
#		Part	:Evil_LightBeams_Chest.part
#		POther	Chest
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet Chest
		POther  Chest
	End

	Event
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrail01.part
		part	:BodySmokeTrailAdd.part
		part	:FireEmbersBody01.part
		Pmagnet Neck
		POther  Neck
	End

	Event
		Type	Local
		At	Chest
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet Hips
		POther  Hips
	End

	Event
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet UArmL
		POther  UArmL
	End

	Event
		Type	Local
		At	Hips
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbersBody01.part
		Pmagnet UarmR
		POther  UarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmL
	End

	Event
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmR
		POther  LarmR
	End

	Event
		Type	Local
		At	UarmR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet UarmR
	End

	Event
		Type	Local
		At	UarmL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	LarmL
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
	End

	Event
		Type	Local
		At	LarmR
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
	End

	Event
		Type	Local
		At	WepL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet LarmL
		POther  LarmL
	End

	Event
		Type	Local
		At	WepR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet LarmR
		POther  LarmR
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	LeftULegGlow
		Type	Local
		At	UlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet Llegl
		POther  Llegl
	End

	Event
		ENAME	RightULegGlow
		Type	Local
		At	UlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		Pmagnet LlegR
		POther  LlegR
	End

	Event
		Type	Local
		At	LlegL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
		Pmagnet FootL
		POther  FootL
	End

	Event
		Type	Local
		At	LlegR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:BodySmokeTrailSmall01.part
		part	:BodySmokeTrailSmallAdd.part
		Pmagnet FootR
		POther  FootR
	End

	Event
		Type	Local
		At	FootR
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet FootR
	End

	Event
		Type	Local
		At	FootL
		part	:BodySmoke01.part
		part	:BodySmokeAdd.part
		part	:FireEmbers01.part
		Pmagnet FootL
	End
End

#############################################################

#Condition
#	On	Cycle
#	Time	30
#
#	Event
#		Type	SetLight
#		BHVR	:LightPulse_Dark.bhvr
#		LifeSpan 300
#	End
#End

#############################################################

End