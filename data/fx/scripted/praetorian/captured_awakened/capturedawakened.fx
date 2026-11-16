#############################################################
## Captured Penelope Yin FX
#############################################################

FxInfo
#
#LifeSpan 200

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_CapturedAwakened_Loop 50 50 .8
	End
End


Condition
	On 	Time
	Time 	10

	Event
		EName 	Bondage
		Type	Start
		At	Waist
		BhvrOverride
			Pyrrotate	20 0 0
			PositionOffset	0 -4.5 0
		end
##		Part1	:Shackle_Ring1.part
		Part1	:Shackle_Ring2.part
		Lifespan	0
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	WaistSpin
		Type	Local
		At	Waist
		BhvrOverride
			Spin	0 0.125 0
		End
	End

#	Event
#		EName	WaistOffset1
#		Type	Local
#		At	WaistSpin
#		BhvrOverride
#			PositionOffset	1.25 0 0
#		End
#		#Geom	GEO_FX_TestSphere
#	End
#
#	Event
#		EName	WaistOffset2
#		Type	Local
#		At	WaistSpin
#		BhvrOverride
#			PositionOffset	-0.75 0 -1
#		End
#		#Geom	GEO_FX_TestSphere
#	End
#
#	Event
#		EName	WaistOffset3
#		Type	Local
#		At	WaistSpin
#		BhvrOverride
#			PositionOffset	-0.75 0 1
#		End
#		#Geom	GEO_FX_TestSphere
#	End
End

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	:BodyGlow_Large1.part
#		Part1	:BodyGlow_Large2.part
#		Part1	:BodyGlow_Rays1.part
		Part1	:BodyGlow_Rays2.part
		pMagnet	Chest
	End

End

## GROUND CAGE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	CageSpin
		Type	Local
		At	Root
		BhvrOverride
			Spin	0 0.125 0
		End
	End



	Event
		EName	CageOffset3
		Type	Local
		At	CageSpin
		BhvrOverride
			PositionOffset	-1.25 0 1.875
		End
		#Geom	GEO_FX_TestSphere
	End

End



#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Posit
		At	chest
#		Part1   :Awakened_Lightning.part


	End
End

condition
	On 	Time
	Time 	5

	Event
		Type	StartPositOnly
		At	chest
		BhvrOverride
			PositionOffset	0 -4 0
		end
		Part1   :Awakened_LightningSmall.part


	End
End



#############################################################

End