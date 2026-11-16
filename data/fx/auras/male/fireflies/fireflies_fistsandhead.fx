#############################################################
## Fireflies_Body.fx
#############################################################

FxInfo

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
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -0.05
		End
		Part	:EyeGlow_Pupil.part
		Part	:EyeGlow_BG.part

	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	Head
		BhvrOverride
			PositionOffset	1.5 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Head_Trail.part
		PMagnet	Head
	End

	Event
		Type	PositOnly
		At	WepL
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Hand_Trail.part
		PMagnet	WepL
	End

	Event
		Type	PositOnly
		At	WepR
		BhvrOverride
			PositionOffset	1.0 0.0 0.0
		End
		part	:Fireflies_Head.part
		Part	:Fireflies_Hand_Trail.part
		PMagnet	WepR
	End
End

#############################################################

End