#########################################################
##	
##

FxInfo

LifeSpan	200

Input  
	InpName	origin
End

Input  
	InpName	chest
End

Input  
	InpName	hips
End

Input  
	InpName	target
End

Input  
	InpName	head
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End
##################################
Condition
	On 	Time
	Time 	23

	Event
		EName	Prime
		Type	start
		At	hips
		Geom	Fx_Spike01
		Bhvr	behaviors/Quillprojectile2.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part3	:QuillTrailGreenBits.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Prime1
		Type	start
		At	Spike10
		Geom	Fx_Spike01
		Bhvr	behaviors/Quillprojectile2.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part3	:QuillTrailGreenBits.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
	End

End

Condition
	On 	Time
	Time 	27

	Event
		EName	Prime2
		Type	start
		At	Spike11
		Geom	Fx_Spike01
		Bhvr	behaviors/Quillprojectile2.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part3	:QuillTrailGreenBits.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
	End

End


Condition
	On 	Time
	Time 	30

	Event
		EName	Prime4
		Type	start
		At	Spike1
		Geom	Fx_Spike01
		Bhvr	behaviors/Quillprojectile2.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:QuillTrailGreenBits.part
		Part2	:QuillTrailBlackBits.part
		Part3	:QuillTrailGreenBits.part
		Part4	:QuillTrailLightGreenBits.part
		Part5	:QuillTrailLightBlackBits.part
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		Ename	Spike1
		Type	Local
		At	chest
		Bhvr	behaviors/poisonspikeLarge.bhvr
		Geom	SpikeA
		
	End

	Event
		Ename	Spike2
		Type	Local
		At	chest
		Bhvr	behaviors/poisonspikeLarge.bhvr
		Geom	SpikeB
			
	End

	Event
		Ename	Spike3
		Type	Local
		At	chest
		Bhvr	behaviors/poisonspikeLarge.bhvr
		Geom	SpikeC
			
	End

	Event
		Ename	Spike4
		Type	Local
		At	chest
		Bhvr	behaviors/poisonspikeLarge.bhvr
		Geom	SpikeD
			
	End

	Event
		Ename	Spike5
		Type	Local
		At	chest
		Bhvr	behaviors/poisonspikeLarge.bhvr
		Geom	SpikeE
			
	End

	Event
		Ename	Spike6
		Type	Local
		At	chest
		Bhvr	behaviors/PoisonSpikeSmall2.bhvr
		Geom	SpikeF
			
	End

	Event
		Ename	Spike7
		Type	Local
		At	chest
		Bhvr	behaviors/PoisonSpikeSmall.bhvr
		Geom	SpikeG
			
	End

	Event
		Ename	Spike8
		Type	Local
		At	UarmL
		Bhvr	behaviors/PoisonSpikeSmall.bhvr
		Geom	SpikeH
			
	End

	Event
		Ename	Spike9
		Type	Local
		At	UarmR
		Bhvr	behaviors/PoisonSpikeSmall.bhvr
		Geom	SpikeI
			
	End

	Event
		Ename	Spike10
		Type	Local
		At	UarmL
		Bhvr	behaviors/PoisonSpikeSmall.bhvr
		Geom	SpikeJ
			
	End

	Event
		Ename	Spike11
		Type	Local
		At	UarmR
		Bhvr	behaviors/PoisonSpikeSmall.bhvr
		Geom	SpikeK
			
	End

End


Condition
	On 	PrimeHit
	Event

		Type	Destroy
		Ename	Prime			
	End
	

End

Condition
	On 	Prime1Hit
	Event

		Type	Destroy
		Ename	Prime1			
	End
	

End

Condition
	On 	Prime2Hit
	Event

		Type	Destroy
		Ename	Prime2			
	End
	

End

Condition
	On 	Prime3Hit
	Event

		Type	Destroy
		Ename	Prime3			
	End
	

End

Condition
	On 	Prime4Hit
	Event

		Type	Destroy
		Ename	Prime4			
	End
	

End

End				