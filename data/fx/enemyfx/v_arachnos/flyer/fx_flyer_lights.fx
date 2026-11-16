#########################################################
##	template

FxInfo

Flags  InheritAlpha   InheritAnimScale


########  Antannae  #############################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part	:Flyer_Antannae_Blink.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_01.bhvr	
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part	:Flyer_Antannae_Blink.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_02.bhvr	
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part	:Flyer_Antannae_Blink_02.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_03.bhvr	
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	Aura
		Type	Local
		At	Hips	
		part	:Flyer_Antannae_Blink_02.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_04.bhvr	
	End

End

End

