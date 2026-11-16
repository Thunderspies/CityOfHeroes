#########################################################
##	template

FxInfo

Flags  InheritAlpha  


########  Antannae  #############################################




Condition
	On 	Time
	Time 	0

	Event
		EName	Aura
		Type	Local
		At	hips	
		bhvroverride
			scale .1 .1 .1
		end
	End
End
Condition
	On 	Time
	Time 	0

	Event
		EName	Aura
		Type	Local
		At	Aura	
		part	:Flyer_Antannae_Blink.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_01.bhvr	
		#Sound	arachnosflyerbeep2_loop 120 110 .33
	End
End

Condition
	On 	Time
	Time 	0

	Event
		EName	Aura
		Type	Local
		At	Aura	
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
		At	Aura	
		part	:Flyer_Antannae_Blink_02.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_03.bhvr	
		#Sound	arachnosflyerbeep_loop 120 110 .33
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	Aura
		Type	Local
		At	Aura	
		part	:Flyer_Antannae_Blink_02.part
		part	:Flyer_Antannae_Blink_Soft.part
		bhvr	:Flyer_Antannae_04.bhvr	
	End

End

End

