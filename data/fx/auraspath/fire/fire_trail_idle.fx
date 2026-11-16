#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################
## IDLE

Condition
	On	Cycle
	Time	1

	Event
		Ename 	BigFire
		Type	Local
		At	Root
		Pother	Head
		#Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 1 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Idle_Alpha.part
		Part	:FireTrail_Idle_Additive.part
		Part	:FireTrail_Idle_CoreAdd.part
		Lifespan 	6
		LifespanJitter	4
	End
End

############

End