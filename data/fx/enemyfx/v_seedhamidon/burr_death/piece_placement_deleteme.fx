#############################################################
## Disintegration_Continuing.fx
#############################################################

FxInfo
Flags DontSuppress

#Lifespan 90

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On 	Time
	Time	0
	Repeat	5

	Event
		Ename	RadiusFxScale
		Type	Local
		At	Chest
		Bhvr	behaviors/FadeOut.bhvr
	#	Flags	AdoptParentEntity
	End

	Event
		Ename	SeedChunk01
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_01
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	2.6 1 -1
		End
	End

	Event
		Ename	SeedChunk02
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_02
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	3.5 -1.2 0.4
		End
	End

	Event
		Ename	SeedChunk03
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_03
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	0 2.3 4.1
		End
	End

	Event
		Ename	SeedChunk04
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_04
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	0 3.2 1.6
		End
	End

	Event
		Ename	SeedChunk05
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_05
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	0 3.5 -1.0
		End
	End

	Event
		Ename	SeedChunk06
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_06
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	0 2.4 -2.1
		End
	End

	Event
		Ename	SeedChunk07
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_07
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	-2.6 1 -1
		End
	End

	Event
		Ename	SeedChunk08
		Type	Local
		At	RadiusFxScale
		Geom	FX_HamidonSeedPiece_08
		bhvrOverride
			StartColor	36, 32, 33
			PositionOffset	-3.5 -1.2 0.4
		End
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		400
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

End