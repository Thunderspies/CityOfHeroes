#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	root
End

LifeSpan	200

##################################
Condition
	On	Time
	Time	0
		

	Event	

		ENAME	Puddle
		Type	local
		At	root

		
		Part2	:DarkRealmPuddle1.part
		
	End
	

	Event	

		ENAME	Tenticle1
		Type	local
		At	root

		Geom	Tenticle01a
		Bhvr	behaviors/TenticleScale.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

	Event	

		ENAME	Tenticle2
		Type	local
		At	root

		Geom	Tenticle02a
		Bhvr	behaviors/TenticleScale.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

	Event	

		ENAME	Tenticle3
		Type	local
		At	root

		Geom	Tenticle03a
		Bhvr	behaviors/TenticleScale.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

	Event	

		ENAME	Tenticle4
		Type	local
		At	root

		Geom	Tenticle04a
		Bhvr	behaviors/TenticleScale.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

	Event	

		ENAME	Tenticle5
		Type	local
		At	root

		Geom	Tenticle05a
		Bhvr	behaviors/TenticleScale2.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

	Event	

		ENAME	Tenticle6
		Type	local
		At	root

		Geom	Tenticle06a
		Bhvr	behaviors/TenticleScale3.bhvr
		Part2	:DarkRealmPuddle1.part
		
	End

End

End				