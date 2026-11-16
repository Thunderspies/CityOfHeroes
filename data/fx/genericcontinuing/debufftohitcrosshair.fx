#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Cycle
	Time	180

	Event
		ENAME	Chestnode
		Type	Local
		At	Origin
		Part	:DeBuffToHitCrosshair01.part
		Lifespan 90
	End
End

#############################################################

End