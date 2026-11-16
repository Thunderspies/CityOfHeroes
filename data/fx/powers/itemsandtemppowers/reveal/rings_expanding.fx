#########################################################
##	template

FxInfo
Lifespan 30
#########################################################
#################### GEYSER ########################
#########################################################


Condition
	On	Cycle
	Time	10


	Event
		Ename 	Circle
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp2.bhvr
		BhvrOverride
			Alpha		255
			PyrRotateJitter 0 0 0
			StartColor 100 100 255
			SplatFlags ADDITIVE
		End
		Splat	Circle_Thin.tga
		Lifespan 10
	End

	Event
		Ename 	Circle
		Type	StartPositOnly
		At	Root
		Bhvr	:Splat_FootStomp2.bhvr
		BhvrOverride
			PyrRotateJitter 0 0 0
			StartColor 100 100 255
			SplatFlags ADDITIVE
		End
		Splat	Mito_Outer_01.tga
		Lifespan 10
	End
End






