#############################################################
## Alpha_Hair_Male.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On	Time
	Time	0

#############################################################

	Event
		EName	HeadAnchor
		Type	Local
		At	Head
		Bhvr	behaviors/GenericParticleFade.bhvr

		BhvrOverride
			PositionOffset	0.0 0.175 0.0
		End
	End

	Event
		EName	Ring
		Type	Local
		At	HeadAnchor
		Bhvr	behaviors/GenericParticleFade.bhvr

		Part	:Star_Head.part
		Part	:Star_Hand_Moving.part
		Part	:Glow_Large_Continuing_Chest.part
		Part	:Glow_Ring_Head.part
	End


End

#############################################################

End