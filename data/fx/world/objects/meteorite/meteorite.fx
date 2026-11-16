#########################################################
##	Inferno FX
##
FxInfo





Condition
	On	Time
	Time	0

	Event
		EName 	xs
		Type	local
		At	root

		Geom	Meteorite
		Bhvr	:colorcycle.bhvr
		Sound meteor1_loop 170 170 .99
	End

	Event
		Type	start
		At	origin

		Part	:MeteoriteMist.part
		Part	:MeteoriteSmoke.part

	End

End



End
