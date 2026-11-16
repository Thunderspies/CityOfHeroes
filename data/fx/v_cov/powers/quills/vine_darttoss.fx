#########################################################
##	cot_waterfall_med.fx
##
FxInfo

LifeSpan	200



###########################################################
##################################
##################################

Condition
	On 	Time
	Time 	17



	Event
		
		Type	Local
		At	Origin
		Sound Darttoss 70 70 .7
	End
End


Condition
	On	Time
	Time	19
	

	Event
		EName	Prime
		Type	start
		At	F1_L
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