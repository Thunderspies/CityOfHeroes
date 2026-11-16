#########################################################
##	RadiationAura
FxInfo

##################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	local
		At	origin
		Part1	:RiktiPortalEllipse.part
		
	End

	Event
		Ename	elec
		Type	local
		At	origin
		
		Part2	:RiktiPortalSparks.part

	End

End

Condition
	On	Time
	Time	18

	Event
		EName	AntiMatter
		Type	local
		At	origin
		Part3	:RiktiPortalSpecks.part
		Part4	:RiktiPortalStreaks.part
		Sound Materialize1 500.0 500.0 .5
		Part2	:RiktiPortalRings.part
		Part2	:PortalRays.part
		Part3	:PortalStreakRays.part
	End

End

Condition
	On	Time
	Time	24
	Event
		EName	Ring2
		Type	local
		At	origin
		Part3	:RiktiPortalEllipsePulse.part
		Sound glow6_loop 70 70 .67
		
	End

End


Condition
	On	Time
	Time	32

	Event
		EName	Ring4
		Type	local
		At	origin
		Part3	:RiktiEllipsePulseShrinkAngles.part
		
	End

End


End				