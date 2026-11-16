#########################################################
##	Fireweapon
########################################################
FxInfo

######################################################################


LifeSpan	13



Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	root

		BhvrOverride
			
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End
	
	Event
		Type	local
		At	root

		BhvrOverride
			PYRrotate		0 180 0
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End

	Event
		Type	local
		At	root

		BhvrOverride
			
			FadeInLength		0
			FadeOutLength		300

		End

		Geom    _arach_prisonDoorFrame_01

		LifeSpan	99999
	End

End


Condition
	On 	Time
	Time 	5


	Event
		Type	local
		At	root

		BhvrOverride
			
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End
	
	Event
		Type	local
		At	root

		BhvrOverride
			PYRrotate		0 180 0
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End

End

Condition
	On 	Time
	Time 	8


	Event
		Type	local
		At	root

		BhvrOverride
			
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End
	
	Event
		Type	local
		At	root

		BhvrOverride
			PYRrotate		0 180 0
			FadeInLength		10
			FadeOutLength		20

		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	3
	End

End


Condition
	On 	Time
	Time 	13


	Event
		Type	local
		At	root

		Bhvr	:doorFade.bhvr

		Geom    _arach_prisonDoorField_01

		LifeSpan	1
	End
	
	Event
		Type	local
		At	root

		Bhvr	:doorFade.bhvr

		BhvrOverride
			PYRrotate		0 180 0
#			FadeInLength		10
#			FadeOutLength		60
#
		End

		Geom    _arach_prisonDoorField_01

		LifeSpan	1
	End

End


End
##################################

