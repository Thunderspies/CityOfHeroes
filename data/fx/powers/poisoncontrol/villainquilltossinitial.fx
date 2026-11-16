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


Condition
	On 	Time
	Time 	20



	Event
		
		Type	Local
		At	Origin
		Sound treetoss3 70 70 .75
	End
End

###########################################################

Condition
	On	Time
	Time	65
	

	Event
		EName	Prime
		Type	start
		At	WepR
		Geom	SpineProjectile		#Fx_Spike01
		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		Part	:VillainQuillTrailGreenBitsA.part
		Part	:VillainQuillTrailGreenBitsA.part
		Part1	:VillainQuillTrailGreenBitsA.part
		Part2	:VillainLightCloudSpecks.part
		Part4	:VillainQuillTrailLightGreenBitsA.part
		#Part5	:QuillTrailLightBlackBits.part
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