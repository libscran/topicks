<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>topicks.hpp</name>
    <path>topicks/</path>
    <filename>topicks_8hpp.html</filename>
    <class kind="struct">topicks::PickTopGenesOptions</class>
  </compound>
  <compound kind="file">
    <name>TopQueue.hpp</name>
    <path>topicks/</path>
    <filename>TopQueue_8hpp.html</filename>
    <class kind="struct">topicks::TopQueueOptions</class>
    <class kind="class">topicks::TopQueue</class>
  </compound>
  <compound kind="struct">
    <name>topicks::PickTopGenesOptions</name>
    <filename>structtopicks_1_1PickTopGenesOptions.html</filename>
    <templarg>typename Stat_</templarg>
    <member kind="variable">
      <type>std::optional&lt; Stat_ &gt;</type>
      <name>bound</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>afce1d913773d4cca49ccc07a7f952f88</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>open_bound</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>af66b687b1bc89dee122ab85a27afdf3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>keep_ties</name>
      <anchorfile>structtopicks_1_1PickTopGenesOptions.html</anchorfile>
      <anchor>a2db78cb727b9c6e9c687127ee5bbbb69</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>topicks::TopQueue</name>
    <filename>classtopicks_1_1TopQueue.html</filename>
    <templarg>typename Stat_</templarg>
    <templarg>typename Index_</templarg>
    <member kind="function">
      <type></type>
      <name>TopQueue</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a3eb293431b5b55725148d1f24443a90f</anchor>
      <arglist>(const Index_ top, const bool larger, const TopQueueOptions&lt; Stat_ &gt; &amp;options)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a26e76a49e4e36fb1fe734bce3222e618</anchor>
      <arglist>(std::pair&lt; Stat_, Index_ &gt; gene)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>emplace</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a396044984a5dde2833fb7a1ec0048035</anchor>
      <arglist>(Stat_ statistic, Index_ index)</arglist>
    </member>
    <member kind="function">
      <type>Index_</type>
      <name>size</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>afc4a3821ae393344b71fbedddfb481ee</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>empty</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>ae94d3c9521913c63e610c0ee07b1fcfe</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const std::pair&lt; Stat_, Index_ &gt; &amp;</type>
      <name>top</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a3eb0f3afc1a9542736255b808015fb93</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop</name>
      <anchorfile>classtopicks_1_1TopQueue.html</anchorfile>
      <anchor>a36c1af55c44cbc3f5f2cbdc001435b2c</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>topicks::TopQueueOptions</name>
    <filename>structtopicks_1_1TopQueueOptions.html</filename>
    <templarg>typename Stat_</templarg>
    <member kind="variable">
      <type>std::optional&lt; Stat_ &gt;</type>
      <name>bound</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>a6a382963aafef610e30c921c0705c4e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>open_bound</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>a80d91d1ed3ae89aa9b8e92fb050b350f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>keep_ties</name>
      <anchorfile>structtopicks_1_1TopQueueOptions.html</anchorfile>
      <anchor>aa40aaef482e1df7862067b5a71b58a9f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Pick top genes</title>
    <filename>index.html</filename>
    <docanchor file="index.html" title="Pick top genes">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>
