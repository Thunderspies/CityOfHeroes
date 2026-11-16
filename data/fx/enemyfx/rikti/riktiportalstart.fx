#########################################################
##	RadiationAura
FxInfo

LifeSpan 200
##################################
Condition
	On	Time
	Time	0

	Event
		EName	Hook
		Type	local
		At	root
		Geom	OverHeadDummy01
	End

	Event
		Ename	Hookup
		Type	start
		At	root
		Geom	RootToChestAdjustment
		
	End

	Event
		EName 	Prime
		Type	Local
		At		Origin 
		Bhvr	:portalfadein.bhvr
		Flags	AdoptParentEntity
	End

End

Condition
	On	Time
	Time	3

	Event
		EName	Ring
		Type	local
		At	Hookup
		altpiv	1
		#Part1	:RiktiPortalEllipse.part
		
	End

	Event
		Ename	Upwards
		Type	local
		At	hookup
		altpiv	1
		Part1	:UpwardStreaks.part
	End

	Event
		Ename	elec
		Type	local
		At	chest
		
		Part2	:RiktiPortalSpark.part

	End

End

Condition
	On	Time
	Time	6

	Event
		EName	AntiMatter
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalSpecks.part
		Part4	:RiktiPortalStreaks.part
		Sound Materialize1 500.0 500.0 .5
		#Part2	:RiktiPortalRings.part
		Part2	:PortalRays.part
		Part3	:PortalStreakRays.part
	End

End

Condition
	On	Time
	Time	12
	Event
		EName	Ring2
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiPortalEllipsePulse.part
		
	End

End


Condition
	On	Time
	Time	20

	Event
		EName	Ring4
		Type	local
		At	Hookup
		altpiv	1
		Part3	:RiktiEllipsePulseShrinkAngles.part
		
	End

End


End				