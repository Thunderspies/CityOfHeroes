#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	2

	Event
		Ename 	BigFire
		Type	Local
		At	Root
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2.part
		Part	:FireTrail_CoreAdd2.part
		Lifespan 5
	End
End

#########################################################

End