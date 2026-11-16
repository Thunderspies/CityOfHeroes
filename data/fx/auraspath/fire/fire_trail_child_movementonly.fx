#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Ename	FlareUp
		Type	Local
		At	Root
		Part	:FireTrail_Alpha_out.part
		Part	:FireTrail_Alpha_Additive_out.part
		Part	:FireTrail_CoreAdd_out.part
	End
End

Condition
	On	Cycle
	Time	1

	Event
		Ename 	BigFire
		Type	Local
		At	Root
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 -1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2_MovementOnly.part
		Part	:FireTrail_Alpha2_Additive_MovementOnly.part
		Part	:FireTrail_CoreAdd2_MovementOnly.part
		Lifespan 	6
		LifespanJitter	4
	End
End

#########################################################

End