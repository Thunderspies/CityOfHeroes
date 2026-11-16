#########################################################
##	forceBubble
########################################################
FxInfo

Input
	Inpname	Root
End

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	lazer
		Type	start
		At	root

		Bhvr	Behaviors/caltropsFadein.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Caltrops


	End


End


End