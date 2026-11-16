#############################################################
## HEADER
#############################################################

FxInfo

#############################################################

Condition
         On	Time
         Time	15

         Event
		Type     Local
                At       Origin
                Bhvr     behaviors\soundFade4.bhvr
                Flags    PowerLoopingSound
                Sound InvCube_loop 40 40 .36
		Lifespan 120
       End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Mystic
		bhvr	Behaviors\InventionWidget.bhvr
		Geom	GEO_InventionCube
	End

	Event
		Type	Local
		At	Mystic
		bhvr	Behaviors\InventionWidget.bhvr
		Geom	GEO_InventionBeams
	End
End

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Mystic
		bhvr	Behaviors\InventionWidget.bhvr
		Part1	:InventionWidgetRaysCyan.part
		Part2	:InventionWidgetRaysBlue.part
	End
End

Condition
	On 	Cycle
	Time 	15

	Event
		Type	Local
		At	Mystic
		Bhvr	Behaviors\InventionWidgetMotes.bhvr
		Part1	:InventionWidgetMotes.part
		LookAt	Mystic
		Magnet	Mystic
		Lifespan 30
	End
End

#############################################################

End