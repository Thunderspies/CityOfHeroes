#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200

Input  
	InpName	Origin
End


Input  
	InpName	Target
End

Input  
	InpName	WepR
End


###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	20



	Event
		
		Type	Local
		At	Origin
		Sound thorntoss3 70 70 .79
	End
End


Condition
	On	Time
	Time	42
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	SpineProjectile2		#Fx_Spike01
		Bhvr	behaviors/SpineProjectile.bhvr
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