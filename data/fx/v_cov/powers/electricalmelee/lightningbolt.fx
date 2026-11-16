#########################################################
##	forceBubble
########################################################
FxInfo


LifeSpan	200

########################################################


Condition
#	On 	Time
#	Time 	25

	Event
		Ename	Prime
		Type	start
		At	HandR	
		
		BhvrOverride
			
			FadeOutLength		220

		End

		Bhvr	:LightningBoltBeam.bhvr
		Geom	LightningBoltRigging
		
		Magnet	Target
		LookAt	Target	
	End		

End

Condition
#	On 	Time
#	Time 	33

	Event
#		Ename	Node08
		Type	local
		At	T_Chest
		BhvrOverride
						
			FadeOutLength		220

		End
		Part	:ElectricityBlastHaze.part
		Part	:ChargedBolt03.part
		Part	:ChargedBolt04.part
		Part	:ChargedBolt06.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node08
	End

End	

Condition
#	On 	Time
#	Time 	33

	Event
		Ename	Node08
		Type	local
		At	Prime
		Altpiv	9
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node07
	End

End	


Condition
#	On 	Time
#	Time 	32

	Event
		Ename	Node07
		Type	local
		At	Prime
		Altpiv	8
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt02.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node06
	End

End

Condition
#	On 	Time
#	Time 	31

	Event
		Ename	Node06
		Type	local
		At	Prime
		Altpiv	7
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt04.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node05
	End

End

Condition
#	On 	Time
#	Time 	30

	Event
		Ename	Node05
		Type	local
		At	Prime
		Altpiv	6
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt01.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node04
	End

End

Condition
#	On 	Time
#	Time 	29

	Event
		Ename	Node04
		Type	local
		At	Prime
		Altpiv	5
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt02.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node03
	End

End

Condition
#	On 	Time
#	Time 	28

	Event
		Ename	Node03
		Type	local
		At	Prime
		Altpiv	4
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt02.part
		Part5	:ChargedBolt04.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node02
	End

End

Condition
#	On 	Time
#	Time 	27

	Event
		Ename	Node02
		Type	local
		At	Prime
		Altpiv	3
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt01.part
		Part5	:ChargedBolt04.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Node01
	End

End

Condition
#	On 	Time
#	Time 	26

	Event
		Ename	Node01
		Type	local
		At	Prime
		Altpiv	2
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		Part	:ElectricityBlastHaze.part
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt02.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part	:ChargedBoltFade03.part
		POther  Home
	End

	Event
		Ename	Home
		Type	local
		At	Prime
		altpiv	1
		BhvrOverride
			
			StartJitter		.25 .25 .25

		End
		
		Part	:ElectricityBlastHaze.part
		Part	:ChargedBoltFade01.part
		Part	:ChargedBoltFade02.part
		Part1	:ChargedBolt06.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		Part	:ChargedBoltFade03.part
		POther  HandR	
	End		
	
End


#############################################################################
##Hands
############################################################

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End