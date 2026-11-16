#############################################################
## Captured Penelope Yin FX
#############################################################

FxInfo

LifeSpan 50

## FX ON PENELOPE ###########################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	FW_CapturedAwakened_Static_01 50 50 .8
		Sound 	FW_CapturedAwakened_Static_02 50 50 .8
		Sound 	FW_CapturedAwakened_Static_03 50 50 .8
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Bondage
		Type	Local
		At	Waist

#		Part1	:Shackle_Ring1.part
#		Part1	:Shackle_Ring2.part
		Lifespan	0
	End
End




## GROUND CAGE ###########################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		EName	CageSpin
#		Type	Local
#		At	Root
#		BhvrOverride
#			Spin	0 0.125 0
#		End
#	End
#
#
#End



#############################################################


condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	chest
		BhvrOverride
			PositionOffset	0 -8 0
		end
		Part1   :Awakened_LightningSmallBright.part

		Lifespan	30

	End
End



#############################################################

End