#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	1

	Event
		Ename 	BigFire
		Type	Local
		At	Root
		Bhvr	:FireShrink.bhvr
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate 	0 0.5 0
		End
		Part	:FireTrail_Alpha2.part
		Part	:FireTrail_Alpha2_Additive.part
		Part	:FireTrail_CoreAdd2.part
		Lifespan 	6
		LifespanJitter	4
	End
End


#Condition
#	On	Cycle
#	Time	1
#
#	Event
#		Ename 	BigFire
#		Type	Local
#		At	Origin
#		Part	:FireTrail_Flying_Alpha.part
#		Part	:FireTrail_Flying_Additive.part
#		Part	:FireTrail_flying_CoreAdd.part
#		Lifespan 	6
#		LifespanJitter	1
#	End
#End

#########################################################

End