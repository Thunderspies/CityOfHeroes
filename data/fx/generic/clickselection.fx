#########################################################
##	TemperatureControl
########################################################

FxInfo

##########################

Condition
	On 	Time
	Time 	0

	
	Event
		Ename	Scaller
		Type	Local
		At	origin

		Geom	ArrowScaller
		BHVR	:Scale.bhvr
	End


	Event
		Type	Local
		At	Scaller
		Altpiv	1
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow
		Lifespan 15

	End

	Event
		Type	Local
		At	Scaller
		Altpiv	2
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow02
		Lifespan 15

	End

	Event
		Type	Local
		At	Scaller
		Altpiv	3
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow03
		Lifespan 15

	End

	Event
		Type	Local
		At	Scaller
		Altpiv	4
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow04
		Lifespan 15

	End

End


Condition
	On 	Time
	Time 	6

	
	Event
		Ename	Scaller2
		Type	Local
		At	origin

		Geom	ArrowScaller
		BHVR	:Scale.bhvr
	End


	Event
		Type	Local
		At	Scaller2
		Altpiv	1
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow
		Lifespan 20
	End

	Event
		Type	Local
		At	Scaller2
		Altpiv	2
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow02
		Lifespan 20

	End

	Event
		Type	Local
		At	Scaller2
		Altpiv	3
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow03
		Lifespan 20

	End

	Event
		Type	Local
		At	Scaller2
		Altpiv	4
		BHVR	:Scaleup.bhvr
		Geom	ClickSelectionArrow04
		Lifespan 20

	End

End

####################################################################################

End