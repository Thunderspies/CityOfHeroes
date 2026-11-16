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
		Sound Quills5 70 70 .7
	End
End


Condition
	On	Time
	Time	42
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	ThornProjectile		#Fx_Spike01
		Bhvr	behaviors/ThornProjectile.bhvr
		Magnet	Target
		LookAt	Target
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailGreenBitsA.part
		Part	:QuillTrailBlackBitsA.part
		Part	:QuillTrailLightGreenBitsA.part
		Part	:QuillTrailLightGreenBitsA.part
		Part	:QuillTrailLightBlackBitsA.part
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