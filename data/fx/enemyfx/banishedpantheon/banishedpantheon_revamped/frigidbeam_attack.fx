############################################################
##	Fireweapon
###########################################################



FxInfo

Flags	InheritAnimScale
Lifespan	200

#################################################################################
#############################  P R I M E    H I T  ##############################
#################################################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	BPR_FrigidBeam_VOX_01 100 100 .8
	End
End

Condition
	On 	Time
	Time	20
	Event
		Type	Local
		At	Root
		Sound 	BP_FrigidBeam_Attack_01 200 100 .8
	End
End

Condition
	On Time
	Time 10
	Event
		EName 	Smoke01
		Type	Local
		At	LArmR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	2.0 0.0 0.0
		End
		Part	:Ravager_FrigidBeam_Claws_Core.part
		Part	:Ravager_FrigidBeam_Claws_Boils.part
		POther	HandR
		Lifespan 90
	End



End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	LArmL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-2.0 0.0 0.0
		End
		Part	:Ravager_FrigidBeam_Claws_Core.part
		Part	:Ravager_FrigidBeam_Claws_Boils.part

		POther	HandL
		Lifespan 90
	End
End

Condition
	On	Time
	Time	75

	Event
		EName	HandRAnchor
		Type	Local
		At	HandR
		Geom	FX_ConeOffsets
		BhvrOverride
			Scale		15 15 15
			PYRRotate	0 90 0
			PositionOffset	0 5 0
		End
	End

	Event
		EName	HandRAnchor_Target
		Type	Local
		At	HandRAnchor
		AltPiv	1
		BhvrOverride
			Scale	.1 .1 .1
		End
	End

	Event
		EName 	BallAnchor
		Type	Local
		At	HandR
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:FrigidBeam_Core.part
		Part	:FrigidBeam_Boils.part
		Part	:FrigidBeam_Boils_Beam.part
		Part	:FrigidBeam_IceShards.part
		Lifespan	30
		POther		HandRAnchor_Target

	End


	Event
		EName	HandLAnchor
		Type	Local
		At	HandL
		Geom	FX_ConeOffsets
		BhvrOverride
			Scale		15 15 15
			PYRRotate	0 -90 0
			PositionOffset	0 7 0
		End
	End

	Event
		EName	HandLAnchor_Target
		Type	Local
		At	HandLAnchor
		AltPiv	1
		BhvrOverride
			Scale	.1 .1 .1
		End
	End

	Event
		EName 	BallAnchor
		Type	Local
		At	HandL
	#	Bhvr	behaviors/GenericParticleFade.bhvr
		Part	:FrigidBeam_Core.part
		Part	:FrigidBeam_Boils.part
		Part	:FrigidBeam_Boils_Beam.part
		Part	:FrigidBeam_IceShardsL.part
		Lifespan	30
		POther		HandLAnchor_Target

	End


End

#################################################################################

End