###########################################################

FxInfo
Lifespan 60

###########################################################


Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		SoundNoRepeat 2
		Sound 	Cat_Growl_01 100 100 .8
		Sound 	Cat_Growl_02 100 100 .8
		Sound 	Cat_Growl_03 100 100 .8
		Sound 	Cat_Growl_04 100 100 .8
		Sound 	Cat_Growl_05 100 100 .8
	End
End


Condition
	On 	Time
	Time	40
	Event
		Type	Local
		At	Root
		Sound 	Cat_Roar_01 100 100 .8
		Sound 	Cat_Roar_02 100 100 .8
		Sound 	Cat_Roar_03 100 100 .8
	End
End



Condition
	On	Time
	Time	0

#	Event
#		At Root
#		Type Local
#		Bhvr	:Dust_Expanding.bhvr
#		Part 	:Howl_Wind_Ring.part
#		Lifespan 20
#	End

	Event
		At	Root
		Type	Local
		BhvrOverride
			PositionOffset	0 3 0
			PYRRotate	0 180 0
		End
	#	Part	:Wedge_Wind.part
	#	Part	:Wedge_Wind_Highlights.part

		LifeSpan 20
	End
End

###########################################################

End