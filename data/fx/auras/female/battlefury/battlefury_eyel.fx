#############################################################
## BattleFury.fx
#############################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

### eyes

	Event
		At Eyes
		Type Local
		EName EyeAnchor
		Bhvr behaviors\GenericParticleFade.bhvr
		Geom CircleOfThorns
	End

	Event
		AltPiv 1
		At EyeAnchor
		Type Local
		Bhvr behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.01 0.010 -0.070
		End
		Part Auras\Male\BattleFury\EyeBlobs_L.part
	End
#
#	Event
#		AltPiv 2
#		At EyeAnchor
#		Type Local
#		Bhvr behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset 0 0.01 -0.05
#		End
#		Part :EyeBlobs_R.part
#	End
#
	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -.01
		End
		Part	:Eye_Glow.part
	End
#
#	Event
#		Type	Local
#		At	EyeAnchor
#		AltPiv	2
#		Bhvr	behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0.0 0.0 -.01
#		End
#		Part	:Eye_Glow.part
#	End
End
##################################################################

End
