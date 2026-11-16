#############################################################
## Judgment_Ion_Hit.fx
#############################################################

FxInfo
LifeSpan	90

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Chest
		Bhvr	behaviors/CameraShake05Subtle.bhvr
		Part	:Flash_SparkGroup.part
		Part	:Body_Sparks_Rooted_Small.part
		Part	:Flash_Shockwave_Small.part
		Part	:Flash_Glow_Small.part
		Part	:Hit_Smoke.part
		Part	:Flash_Glow_Linger_Small.part
		Part	:Body_Sparks.part
		POther	Root
		Lifespan	5
	End

	Event
		Type	local
		At	Chest
		Bhvr	behaviors/CameraShake05Subtle.bhvr
		ChildFX	:Judgment_Ion_Continuing.fx
		POther	Root
		Lifespan	90
	End
End

#############################################################

End