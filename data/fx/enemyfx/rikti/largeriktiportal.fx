#########################################################
##	RadiationAura
FxInfo

##################################

Condition
	On	Time
	Time	15

	Event
		EName	Ring
		Type	posit
		At	origin
		Part1	:LargeRiktiPortalEllipse.part
		
	End

	Event
		Ename	elec
		Type	posit
		At	origin
		
		Part2	:LargeRiktiPortalSparks.part

	End

End

Condition
	On	Time
	Time	18

	Event
		EName	AntiMatter
		Type	posit
		At	origin
		Part3	:LargeRiktiPortalSpecks.part
		Part4	:LargeRiktiPortalStreaks.part
		Sound Materialize1 500.0 500.0 .5
		Part2	:LargeRiktiPortalRings.part
		Part2	:LargePortalRays.part
		Part3	:LargePortalStreakRays.part
	End

End

Condition
	On	Time
	Time	24
	Event
		EName	Ring2
		Type	posit
		At	origin
		Part3	:LargeRiktiPortalEllipsePulse.part
		Sound glow6_loop 70 70 .67
		
	End

End


Condition
	On	Time
	Time	32

	Event
		EName	Ring4
		Type	posit
		At	origin
		Part3	:LargeRiktiEllipsePulseShrinkAngles.part
		
	End

End


End				