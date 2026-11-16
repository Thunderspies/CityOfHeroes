#############################################################
## Continuing FX - Sleep
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Headnode
		Type	Local
		At	hair
		Geom	HairToHeadAdjustment
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	FXnode
		Type	Local
		At	Hair
		#AltPiv	1
		part	:SleepZ01.part
		Lifespan 0
	End
End

#############################################################

End