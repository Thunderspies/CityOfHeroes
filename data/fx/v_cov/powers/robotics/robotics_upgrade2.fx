#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 300

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Root
		Sound CompBeep6_loop 120 120 .46
	End

	Event
		Type	Start
		At	Root
		Sound M1.ogg 120 120 .46
		Lifespan 10
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound RobotRepair4b 80 80 .9
	End
End

Condition
	On	Time
	Time	70

	Event
		Type	Local
		At 	Origin
		Sound RobotUpgrade7 80 80 .8
	End
End


#############################################################

Condition
	On	Time
	Time	25

	Event
		ENAME	Prime
		Type	start
		At	root
		BHVR	:Projectile.Bhvr
		Magnet	T_Root
		Lookat	T_Root
	End

	Event
		Type	start
		At	Root
		ChildFx	V_COV\Powers\Robotics\Robotics_Nanomite.fx
		LifeSpan	300
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Type	start
		At	Root

		ChildFx	V_COV\Powers\Robotics\Robotics_Nanomite_Return.fx
		LifeSpan	300
	End
End

#############################################################

End