#########################################################
##	
##

FxInfo
LifeSpan  1000

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

Input  
	InpName	WepR
End

##################################

Condition
	On Time
	Time 0
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		bhvroverride
			positionoffset  0 0 .5
			scale	.01 .01 1
		end
#		part	:bot_activate_Spark.part
		pmagnet	mag
	End
End
Condition
	On Time
	Time 0
	Event
		ename	spinner1
		Type	Local
		At	prime 
		bhvroverride
			spinjitter	0 .1 0
			spin	0 .2 0
		end
	End
	Event	
		ename	mag1
		Type	Local
		At	spinner1 
		bhvroverride
			positionoffset .5 0 0
		end
		part	:bot_activate_SparkLegs.part
#		part	:bot_activate_Spark2.part
	End
	Event	
		ename	mag2
		Type	Local
		At	spinner1
		bhvroverride
			positionoffset -.5 0 0
		end
		part	:bot_activate_SparkLegs.part
#		part	:bot_activate_Spark2.part
	End
End

end
