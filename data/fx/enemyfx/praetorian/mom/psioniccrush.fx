#############################################################
## Captured Penelope Yin FX
#############################################################

FxInfo

LifeSpan 200

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
			PositionOffset	0 -4 0
		end
		Part1	:PsionicCrush_Shackle_Ring2.part
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
End

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part1	:PsionicCrush_BodyGlow_Large1.part
		Part1	:PsionicCrush_BodyGlow_Rays2.part
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
			PositionOffset	0 0 0
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
#		Part1   :PsionicCrush_Lightning.part


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
		Part1   :PsionicCrush_LightningSmall.part


	End
End



#############################################################

End