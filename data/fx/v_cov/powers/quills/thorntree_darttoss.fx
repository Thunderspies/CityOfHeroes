#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200


######## Random Static Node  ###########################

Condition
	On	Time
	Time	0

	Event
		Ename	StaticNode
		Type	Start  
		At	WepR
	End
End




######## Activation flash  ###########################

Condition
	On	Time
	Time	2

	Event
		Type	Start
		At	StaticNode
		part2	POWERS\RadiationControl\RadiationAuraGreenLocal.part
		part3	POWERS\RadiationControl\RadiationAuraYellowLocal.part
		part4	POWERS\RadiationControl\RadiationAuraWhiteLocal.part	
		PMagnet	Chest
		LifeSpan	10
	End
End



###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	13



	Event
		
		Type	Local
		At	Origin
		Sound Darttoss 70 70 .7
	End
End


Condition
	On	Time
	Time	15
	

	Event
		EName	Prime
		Type	start
		At	StaticNode
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors/ThornTree_SpineProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	powers/PoisonControl/QuillTrailBlackBitsA.part
		Part	powers/PoisonControl/QuillTrailLightGreenBitsA.part
		Part	powers/PoisonControl/QuillTrailLightBlackBitsA.part
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
		

	End

End


End			