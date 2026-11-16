#############################################################
## Parent_Hydrant.fx
#############################################################

FxInfo
Lifespan 18000 #150

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Hydrant
		Type	start
		At	Root
		Bhvr	behaviors/FadeOut.bhvr
		BhvrOverride
			FadeOutLength		15
			PhysDensity		2
			StartColor		140 140 140

		End
		Geom	Praet_Street_Details_Hydrant
		Lifespan	18000	#150
	End
End

Condition
	On	Death

	Event
		EName	Hydrant
		Type 	DESTROY
	End

	Event
		Type	Start
		At	Root
		ChildFX	:Hydrant_Random.fx
	End
End

#Condition
#	On	Death
#	Random	1
#
#
#	Event
#		Type	Start
#		At	Root
#		ChildFX	:Parent_Hydrant.fx
#	End
#
#
#	Event
#		Type	Start
#		At	Root
#		ChildFX	:Geyser_Hydrant.fx
#	End
#End

#############################################################

End