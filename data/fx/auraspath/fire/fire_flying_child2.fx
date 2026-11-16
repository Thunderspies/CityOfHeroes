#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo
Lifespan 25

#########################################################

#Condition
#	On	Cucle
#	Time	2
#
#	Event
#		Ename 	BigFire
#		Type	Local
#		At	Root
#		#Until	AIR
#		Bhvr	:FireShrink.bhvr
#		BhvrOverride
#			PositionOffset	0 -1 0
#			PyrRotate 	0 0.5 0
#		End
#		Part	:FireTrail_Alpha2.part
#		Part	:FireTrail_CoreAdd2.part
#		Lifespan 5
#	End
#End

Condition
	On 		Time
	Time 		0
	DoMany		1

	Event
		Ename	Prime
		Type 	StartPositOnly
		At	Origin
		Part	:FireTrail_Flying_Alpha.part
		Part	:FireTrail_Flying_CoreAdd.part
		Lifespan 20
	End
End


#########################################################

End