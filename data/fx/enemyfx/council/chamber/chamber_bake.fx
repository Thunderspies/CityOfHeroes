#########################################################
##	
########################################################
FxInfo

Lifespan	280

##############  Glows    ####################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Glow_L
		Type	Local
		At	FootL
		Geom	Vamp_lite_L
		Bhvr	Behaviors/Bake_fadein.bhvr
	End

	Event
		EName 	Glow_R
		Type	Local
		At	FootR
		Geom	Vamp_lite_R
		Bhvr	Behaviors/Bake_fadein.bhvr
	End

End

##############  Bake Sound   ####################################################

Condition
	On	Time
	Time	14

	Event
		Type Local
		At origin
		Sound vampire1 70 70 .96
		Lifespan	5
	End


End


##############  Door Open Sound   ####################################################

Condition
	On	Time
	Time	82

	Event
		Type Local
		At origin
	##	Sound techdoor3 90 90 .6
		Sound vampdoor1b 70 70 .8
		Lifespan	5
	End


End


##################################


##############  Door close Sound   ####################################################

Condition
	On	Time
	Time	222

	Event
		Type Local
		At origin
	##	Sound techdoor3 90 90 .6
		Sound vampdoor2 70 70 .70
		Lifespan	5
	End


End


End
##################################
