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
		Type	Local
		At	Root
		Sound 	Incarnate_IonJudgement_Hit_01 100 100 .8
	End

	Event
		Type	local
		At	UArmR
		Part	:Flash_Body.part
		Part	:Body_Sparks.part

		POther	LArmR
	End


	Event
		Type	local
		At	UArmL
		Part	:Flash_Body.part
		Part	:Body_Sparks.part

		POther	LArmL
	End

	Event
		Type	local
		At	Hips
		Part	:Flash_Body.part
		Part	:Body_Sparks.part
		POther	Root
	End

	Event
		Type	local
		At	Chest
		Bhvr	behaviors/CameraShake02.bhvr
		Part	:Flash_SparkGroup.part
		Part	:Body_Sparks_Rooted.part
		Part	:Flash_Shockwave.part
		Part	:Flash_Glow.part
		Part	:Hit_Smoke.part
		Part	:Flash_Glow_Linger.part
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