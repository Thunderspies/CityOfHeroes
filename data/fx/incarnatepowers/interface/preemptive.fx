#############################################################
## GCCrushContinuing.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname Hips
End

#############################################################


Condition

	On	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Diamagnetic_Loop 100 100 .3
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150

	End

	Event
		Type	Local
		At	Chest
		part	:Preemptive_Glow_Core.part
		part	:Preemptive_Ring.part
		part	:Preemptive_Dots.part
		part	:Preemptive_Dots_Core.part

		PMagnet	Hips
	End

End


#############################################################

End

